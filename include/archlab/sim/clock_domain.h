#ifndef ARCHLAB_SIM_CLOCK_DOMAIN_H
#define ARCHLAB_SIM_CLOCK_DOMAIN_H

#include "archlab/sim/time.h"

#include <cstdint>
#include <limits>
#include <optional>
#include <stdexcept>

namespace archlab::sim {

class ClockDomain {
public:
    ClockDomain(Tick offset, Tick period) : offset_(offset), period_(period) {
        if (period_ == 0) {
            throw std::invalid_argument("clock period must be greater than zero");
        }
    }

    [[nodiscard]] Tick offset() const noexcept {
        return offset_;
    }

    [[nodiscard]] Tick period() const noexcept {
        return period_;
    }

    [[nodiscard]] Tick edge(std::uint64_t cycle) const {
        constexpr Tick max_tick = std::numeric_limits<Tick>::max();
        if (cycle > (max_tick - offset_) / period_) {
            throw std::overflow_error("clock edge exceeds Tick range");
        }
        return offset_ + cycle * period_;
    }

    [[nodiscard]] bool is_edge(Tick tick) const noexcept {
        return tick >= offset_ && ((tick - offset_) % period_ == 0);
    }

    [[nodiscard]] std::optional<std::uint64_t> cycle_at_or_before(Tick tick) const noexcept {
        if (tick < offset_) {
            return std::nullopt;
        }
        return (tick - offset_) / period_;
    }

    [[nodiscard]] Tick next_edge_at_or_after(Tick tick) const {
        if (tick <= offset_) {
            return offset_;
        }

        const Tick elapsed = tick - offset_;
        const Tick quotient = elapsed / period_;
        const Tick remainder = elapsed % period_;
        return edge(quotient + (remainder == 0 ? 0 : 1));
    }

private:
    Tick offset_;
    Tick period_;
};

} // namespace archlab::sim

#endif // ARCHLAB_SIM_CLOCK_DOMAIN_H
