#include "archlab/stats/stats_manager.h"

#include <ostream>

namespace archlab::stats {

void StatsManager::reset(sim::Tick now) noexcept {
    roi_active_ = false;
    roi_begin_tick_ = now;
    accumulated_ticks_ = 0;
    instructions_ = 0;
    loads_ = 0;
    stores_ = 0;
    branches_ = 0;
}

bool StatsManager::begin_roi(sim::Tick now) noexcept {
    if (roi_active_) {
        return false;
    }
    roi_active_ = true;
    roi_begin_tick_ = now;
    return true;
}

bool StatsManager::end_roi(sim::Tick now) noexcept {
    if (!roi_active_) {
        return false;
    }
    accumulated_ticks_ += now - roi_begin_tick_;
    roi_active_ = false;
    return true;
}

void StatsManager::record_instruction(std::uint64_t count) noexcept {
    if (roi_active_) {
        instructions_ += count;
    }
}

void StatsManager::record_load(std::uint64_t count) noexcept {
    if (roi_active_) {
        loads_ += count;
    }
}

void StatsManager::record_store(std::uint64_t count) noexcept {
    if (roi_active_) {
        stores_ += count;
    }
}

void StatsManager::record_branch(std::uint64_t count) noexcept {
    if (roi_active_) {
        branches_ += count;
    }
}

bool StatsManager::roi_active() const noexcept {
    return roi_active_;
}

std::uint64_t StatsManager::ticks_in_roi(sim::Tick now) const noexcept {
    if (!roi_active_) {
        return accumulated_ticks_;
    }
    return accumulated_ticks_ + (now - roi_begin_tick_);
}

StatsSnapshot StatsManager::snapshot(sim::Tick now) const noexcept {
    return StatsSnapshot{
        .roi_active = roi_active_,
        .roi_ticks = ticks_in_roi(now),
        .instructions = instructions_,
        .loads = loads_,
        .stores = stores_,
        .branches = branches_,
    };
}

void StatsManager::dump(sim::Tick now, std::ostream& stream) const {
    const StatsSnapshot values = snapshot(now);
    stream << "[stats] tick=" << now << '\n'
           << "  roi_active          = " << (values.roi_active ? "true" : "false") << '\n'
           << "  roi_ticks           = " << values.roi_ticks << '\n'
           << "  instructions        = " << values.instructions << '\n'
           << "  loads               = " << values.loads << '\n'
           << "  stores              = " << values.stores << '\n'
           << "  branches            = " << values.branches << '\n';

    if (values.roi_ticks == 0) {
        stream << "  instructions/tick   = n/a\n";
    } else {
        const double rate = static_cast<double>(values.instructions) /
                            static_cast<double>(values.roi_ticks);
        stream << "  instructions/tick   = " << rate << '\n';
    }
}

} // namespace archlab::stats
