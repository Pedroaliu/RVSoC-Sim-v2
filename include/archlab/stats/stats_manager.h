#ifndef ARCHLAB_STATS_STATS_MANAGER_H
#define ARCHLAB_STATS_STATS_MANAGER_H

#include "archlab/sim/time.h"

#include <cstdint>
#include <iosfwd>

namespace archlab::stats {

struct StatsSnapshot {
    bool roi_active = false;
    std::uint64_t roi_ticks = 0;
    std::uint64_t instructions = 0;
    std::uint64_t loads = 0;
    std::uint64_t stores = 0;
    std::uint64_t branches = 0;
};

class StatsManager {
public:
    void reset(sim::Tick now = 0) noexcept;

    bool begin_roi(sim::Tick now) noexcept;
    bool end_roi(sim::Tick now) noexcept;

    void record_instruction(std::uint64_t count = 1) noexcept;
    void record_load(std::uint64_t count = 1) noexcept;
    void record_store(std::uint64_t count = 1) noexcept;
    void record_branch(std::uint64_t count = 1) noexcept;

    [[nodiscard]] bool roi_active() const noexcept;
    [[nodiscard]] std::uint64_t ticks_in_roi(sim::Tick now) const noexcept;
    [[nodiscard]] StatsSnapshot snapshot(sim::Tick now) const noexcept;

    void dump(sim::Tick now, std::ostream& stream) const;

private:
    bool roi_active_ = false;
    sim::Tick roi_begin_tick_ = 0;
    std::uint64_t accumulated_ticks_ = 0;

    std::uint64_t instructions_ = 0;
    std::uint64_t loads_ = 0;
    std::uint64_t stores_ = 0;
    std::uint64_t branches_ = 0;
};

} // namespace archlab::stats

#endif // ARCHLAB_STATS_STATS_MANAGER_H
