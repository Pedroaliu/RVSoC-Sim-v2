#ifndef ARCHLAB_SIM_PORT_H
#define ARCHLAB_SIM_PORT_H

#include "archlab/sim/sim_object.h"

#include <functional>
#include <stdexcept>
#include <string>
#include <utility>

namespace archlab::sim {

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

    void receive(const Message& message) const {
        handler_(message);
    }

private:
    SimObject& owner_;
    std::string name_;
    Handler handler_;
};

} // namespace archlab::sim

#endif // ARCHLAB_SIM_PORT_H
