#ifndef ARCHLAB_SIM_PORT_H
#define ARCHLAB_SIM_PORT_H

#include "archlab/sim/sim_object.h"

#include <functional>
#include <stdexcept>
#include <string>
#include <utility>

namespace archlab::sim {

template <typename Message>
class OutputPort;

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
        return connected_;
    }

    void receive(const Message& message) const {
        handler_(message);
    }

private:
    friend class OutputPort<Message>;

    void bind() {
        if (connected_) {
            throw std::logic_error(
                owner_.name() + "." + name_ + ": input port is already connected");
        }

        connected_ = true;
    }

    SimObject& owner_;
    std::string name_;
    Handler handler_;
    bool connected_ = false;
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
        return peer_ != nullptr;
    }

    void connect(InputPort<Message>& input) {
        if (peer_ != nullptr) {
            throw std::logic_error(
                owner_.name() + "." + name_ + ": output port is already connected");
        }

        input.bind();
        peer_ = &input;
    }

    void send(const Message& message) const {
        if (peer_ == nullptr) {
            throw std::logic_error(
                owner_.name() + "." + name_ + ": output port is not connected");
        }

        peer_->receive(message);
    }

private:
    SimObject& owner_;
    std::string name_;
    InputPort<Message>* peer_ = nullptr;
};

} // namespace archlab::sim

#endif // ARCHLAB_SIM_PORT_H
