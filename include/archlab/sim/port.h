#ifndef ARCHLAB_SIM_PORT_H
#define ARCHLAB_SIM_PORT_H

#include "archlab/sim/sim_object.h"

#include <functional>
#include <stdexcept>
#include <string>
#include <utility>

namespace archlab::sim {

template <typename Message>
class Link;

template <typename Message>
class InputPort {
public:
    using Handler = std::function<void(const Message&)>;

    InputPort(SimObject& owner, std::string name, Handler handler)
        : owner_(owner),
          name_(std::move(name)),
          handler_(std::move(handler)) {
        if (name_.empty()) {
            throw std::invalid_argument("InputPort name must not be empty");
        }

        if (!handler_) {
            throw std::invalid_argument("InputPort handler must not be empty");
        }
    }

    InputPort(const InputPort&) = delete;
    InputPort& operator=(const InputPort&) = delete;
    InputPort(InputPort&&) = delete;
    InputPort& operator=(InputPort&&) = delete;

    [[nodiscard]] const SimObject& owner() const {
        return owner_;
    }

    [[nodiscard]] const std::string& name() const {
        return name_;
    }

    [[nodiscard]] bool is_connected() const {
        return link_ != nullptr;
    }

private:
    friend class Link<Message>;

    void bind(Link<Message>& link) {
        if (link_ != nullptr) {
            throw std::logic_error(
                owner_.name() + "." + name_ + ": input port is already connected");
        }

        link_ = &link;
    }

    void receive(const Message& message) const {
        handler_(message);
    }

    SimObject& owner_;
    std::string name_;
    Handler handler_;
    Link<Message>* link_ = nullptr;
};

template <typename Message>
class OutputPort {
public:
    OutputPort(SimObject& owner, std::string name)
        : owner_(owner),
          name_(std::move(name)) {
        if (name_.empty()) {
            throw std::invalid_argument("OutputPort name must not be empty");
        }
    }

    OutputPort(const OutputPort&) = delete;
    OutputPort& operator=(const OutputPort&) = delete;
    OutputPort(OutputPort&&) = delete;
    OutputPort& operator=(OutputPort&&) = delete;

    [[nodiscard]] const SimObject& owner() const {
        return owner_;
    }

    [[nodiscard]] const std::string& name() const {
        return name_;
    }

    [[nodiscard]] bool is_connected() const {
        return link_ != nullptr;
    }

    void send(const Message& message) const;

private:
    friend class Link<Message>;

    void bind(Link<Message>& link) {
        if (link_ != nullptr) {
            throw std::logic_error(
                owner_.name() + "." + name_ + ": output port is already connected");
        }

        link_ = &link;
    }

    SimObject& owner_;
    std::string name_;
    Link<Message>* link_ = nullptr;
};

template <typename Message>
class Link final : public SimObject {
public:
    explicit Link(std::string name)
        : SimObject(std::move(name)) {
    }

    [[nodiscard]] bool is_connected() const {
        return source_ != nullptr && sink_ != nullptr;
    }

    void connect(OutputPort<Message>& source, InputPort<Message>& sink) {
        if (is_connected()) {
            throw std::logic_error(name() + ": link is already connected");
        }

        if (source.is_connected()) {
            throw std::logic_error(
                source.owner().name() + "." + source.name() +
                ": output port is already connected");
        }

        if (sink.is_connected()) {
            throw std::logic_error(
                sink.owner().name() + "." + sink.name() +
                ": input port is already connected");
        }

        source.bind(*this);
        sink.bind(*this);
        source_ = &source;
        sink_ = &sink;
    }

    [[nodiscard]] const OutputPort<Message>& source() const {
        if (!is_connected()) {
            throw std::logic_error(name() + ": link is not connected");
        }

        return *source_;
    }

    [[nodiscard]] const InputPort<Message>& sink() const {
        if (!is_connected()) {
            throw std::logic_error(name() + ": link is not connected");
        }

        return *sink_;
    }

private:
    friend class OutputPort<Message>;

    void transmit(const Message& message) const {
        if (!is_connected()) {
            throw std::logic_error(name() + ": link is not connected");
        }

        sink_->receive(message);
    }

    OutputPort<Message>* source_ = nullptr;
    InputPort<Message>* sink_ = nullptr;
};

template <typename Message>
void OutputPort<Message>::send(const Message& message) const {
    if (link_ == nullptr) {
        throw std::logic_error(
            owner_.name() + "." + name_ + ": output port is not connected");
    }

    link_->transmit(message);
}

} // namespace archlab::sim

#endif // ARCHLAB_SIM_PORT_H
