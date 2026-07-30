#include "archlab/sim/time.h"
#include "test_framework.h"

namespace {

using archlab::sim::TimePoint;
using archlab::sim::at_tick;
using archlab::sim::next_delta;
using archlab::sim::next_phase;

ARCHLAB_TEST("TimePoint orders tick, phase, and delta") {
    CHECK(at_tick(9) < at_tick(10));
    CHECK(TimePoint{.tick = 10, .phase = 0, .delta = 7} <
          TimePoint{.tick = 10, .phase = 1, .delta = 0});
    CHECK(next_delta(at_tick(10)) == TimePoint{.tick = 10, .phase = 0, .delta = 1});
    CHECK(next_phase(at_tick(10)) == TimePoint{.tick = 10, .phase = 1, .delta = 0});
}

} // namespace
