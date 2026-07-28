#ifndef ARCHLAB_SIM_TIME_H
#define ARCHLAB_SIM_TIME_H

#include <compare>
#include <cstdint>
#include <limits>
#include <stdexcept>

namespace archlab::sim {

using Tick = std::uint64_t;
using Phase = std::uint16_t;
using Delta = std::uint16_t;
using Sequence = std::uint64_t;

struct TimePoint {
    Tick tick = 0;
    Phase phase = 0;
    Delta delta = 0;

    auto operator<=>(const TimePoint&) const = default;
};

struct TimeStamp {
    TimePoint time{};
    Sequence sequence = 0;

    auto operator<=>(const TimeStamp&) const = default;
};

constexpr TimePoint at_tick(Tick tick) noexcept {
    return TimePoint{.tick = tick, .phase = 0, .delta = 0};
}

constexpr TimePoint end_of_tick(Tick tick) noexcept {
    return TimePoint{
        .tick = tick,
        .phase = std::numeric_limits<Phase>::max(),
        .delta = std::numeric_limits<Delta>::max(),
    };
}

inline TimePoint next_phase(TimePoint current) {
    if (current.phase == std::numeric_limits<Phase>::max()) {
        throw std::overflow_error("simulation phase overflow");
    }

    ++current.phase;
    current.delta = 0;
    return current;
}

inline TimePoint next_delta(TimePoint current) {
    if (current.delta == std::numeric_limits<Delta>::max()) {
        throw std::overflow_error("simulation delta overflow");
    }

    ++current.delta;
    return current;
}

} // namespace archlab::sim

#endif // ARCHLAB_SIM_TIME_H
