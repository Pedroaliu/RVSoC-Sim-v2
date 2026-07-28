#include "archlab/devices/magic_device.h"

#include "archlab/stats/stats_manager.h"

#include <ostream>
#include <stdexcept>
#include <utility>

namespace archlab::devices {

MagicDevice::MagicDevice(sim::EventQueue& event_queue, std::string name,
                         stats::StatsManager* stats, std::ostream* output)
    : event_queue_(event_queue), name_(std::move(name)), stats_(stats), output_(output) {
    if (name_.empty()) {
        throw std::invalid_argument("magic device name must not be empty");
    }
}

std::string_view MagicDevice::name() const noexcept {
    return name_;
}

Data MagicDevice::read(Addr offset, platform::AccessSize size) {
    validate_access(offset, size);
    return last_command_;
}

void MagicDevice::write(Addr offset, platform::AccessSize size, Data value) {
    validate_access(offset, size);
    last_command_ = value;

    const auto command = static_cast<MagicCommand>(value);
    if (output_ != nullptr) {
        *output_ << "[magic] tick=" << event_queue_.now() << " command=" << value
                 << " name=" << command_name(command) << '\n';
    }
    execute(command);
}

void MagicDevice::validate_access(Addr offset, platform::AccessSize size) const {
    if (offset != 0 || size != platform::AccessSize::Double) {
        throw std::runtime_error("magic device supports only a 64-bit access at offset zero");
    }
}

void MagicDevice::execute(MagicCommand command) {
    switch (command) {
    case MagicCommand::Nop:
        break;
    case MagicCommand::ResetStats:
        if (stats_ != nullptr) {
            stats_->reset(event_queue_.now());
        }
        break;
    case MagicCommand::DumpStats:
        if (stats_ != nullptr && output_ != nullptr) {
            stats_->dump(event_queue_.now(), *output_);
        }
        break;
    case MagicCommand::StopSimulation:
        event_queue_.request_stop();
        break;
    case MagicCommand::BeginRoi:
        if (stats_ != nullptr) {
            stats_->begin_roi(event_queue_.now());
        }
        break;
    case MagicCommand::EndRoi:
        if (stats_ != nullptr) {
            stats_->end_roi(event_queue_.now());
        }
        break;
    default:
        break;
    }
}

std::string_view MagicDevice::command_name(MagicCommand command) noexcept {
    switch (command) {
    case MagicCommand::Nop:
        return "nop";
    case MagicCommand::ResetStats:
        return "reset-stats";
    case MagicCommand::DumpStats:
        return "dump-stats";
    case MagicCommand::StopSimulation:
        return "stop-simulation";
    case MagicCommand::BeginRoi:
        return "begin-roi";
    case MagicCommand::EndRoi:
        return "end-roi";
    default:
        return "unknown";
    }
}

} // namespace archlab::devices
