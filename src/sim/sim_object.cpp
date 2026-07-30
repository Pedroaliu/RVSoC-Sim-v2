#include "archlab/sim/sim_object.h"

#include <stdexcept>
#include <utility>

namespace archlab::sim {

SimObject::SimObject(std::string name)
    : name_(std::move(name)) {
    if (name_.empty()) {
        throw std::invalid_argument("SimObject name must not be empty");
    }
}

const std::string& SimObject::name() const {
    return name_;
}

LifecycleState SimObject::state() const {
    return state_;
}

void SimObject::initialize() {
    if (state_ != LifecycleState::Constructed) {
        throw std::logic_error(name_ + ": initialize requires Constructed state");
    }

    on_initialize();
    state_ = LifecycleState::Initialized;
}

void SimObject::reset() {
    if (state_ != LifecycleState::Initialized) {
        throw std::logic_error(name_ + ": reset requires Initialized state");
    }

    on_reset();
    state_ = LifecycleState::Ready;
}

void SimObject::startup() {
    if (state_ != LifecycleState::Ready) {
        throw std::logic_error(name_ + ": startup requires Ready state");
    }

    on_startup();
    state_ = LifecycleState::Running;
}

void SimObject::on_initialize() {
}

void SimObject::on_reset() {
}

void SimObject::on_startup() {
}

} // namespace archlab::sim
