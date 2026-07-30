#include "archlab/sim/clock_domain.h"
#include "test_framework.h"

#include <stdexcept>

namespace {

using archlab::sim::ClockDomain;

ARCHLAB_TEST("ClockDomain maps local cycles to global ticks") {
    const ClockDomain clock(3, 10);

    CHECK(clock.edge(0) == 3);
    CHECK(clock.edge(4) == 43);
    CHECK(clock.is_edge(23));
    CHECK(!clock.is_edge(24));
    CHECK(clock.next_edge_at_or_after(24) == 33);
    CHECK(clock.cycle_at_or_before(32).value() == 2);
}

ARCHLAB_TEST("ClockDomain rejects zero period") {
    CHECK_THROWS_AS(ClockDomain(0, 0), std::invalid_argument);
}

} // namespace
