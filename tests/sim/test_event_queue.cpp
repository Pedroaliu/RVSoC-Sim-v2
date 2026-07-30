#include "archlab/sim/event_queue.h"
#include "archlab/sim/time.h"
#include "test_framework.h"

#include <stdexcept>
#include <vector>

namespace {

using archlab::sim::EventQueue;
using archlab::sim::TimePoint;

ARCHLAB_TEST("EventQueue executes events in timestamp order") {
    EventQueue queue;
    std::vector<int> order;

    queue.schedule_abs(30, "event30", [&] { order.push_back(30); });
    queue.schedule_abs(10, "event10", [&] { order.push_back(10); });
    queue.schedule_abs(20, "event20", [&] { order.push_back(20); });
    queue.run_until(100);

    CHECK((order == std::vector<int>{10, 20, 30}));
    CHECK(queue.now() == 100);
}

ARCHLAB_TEST("EventQueue orders phase, delta, and sequence") {
    EventQueue queue;
    std::vector<int> order;

    queue.schedule_at(TimePoint{.tick = 10, .phase = 1, .delta = 0}, "phase1",
                      [&] { order.push_back(4); });
    queue.schedule_at(TimePoint{.tick = 10, .phase = 0, .delta = 1}, "delta1-a",
                      [&] { order.push_back(2); });
    queue.schedule_at(TimePoint{.tick = 10, .phase = 0, .delta = 0}, "delta0",
                      [&] { order.push_back(1); });
    queue.schedule_at(TimePoint{.tick = 10, .phase = 0, .delta = 1}, "delta1-b",
                      [&] { order.push_back(3); });

    queue.run_until(10);
    CHECK((order == std::vector<int>{1, 2, 3, 4}));
}

ARCHLAB_TEST("EventQueue keeps same-time insertion stable") {
    EventQueue queue;
    std::vector<int> order;
    const TimePoint point{.tick = 10, .phase = 0, .delta = 0};

    queue.schedule_at(point, "a", [&] {
        order.push_back(1);
        queue.schedule_at(point, "c", [&] { order.push_back(3); });
    });
    queue.schedule_at(point, "b", [&] { order.push_back(2); });

    queue.run_until(10);
    CHECK((order == std::vector<int>{1, 2, 3}));
}

ARCHLAB_TEST("EventQueue cancels events and resumes after stop") {
    EventQueue queue;
    bool cancelled_fired = false;
    bool later_fired = false;

    const auto handle = queue.schedule_abs(5, "cancelled", [&] { cancelled_fired = true; });
    CHECK(queue.cancel(handle));
    CHECK(!queue.cancel(handle));

    queue.schedule_abs(10, "stop", [&] { queue.request_stop(); });
    queue.schedule_abs(20, "later", [&] { later_fired = true; });
    queue.run_until(100);

    CHECK(!cancelled_fired);
    CHECK(!later_fired);
    CHECK(queue.now() == 10);

    queue.clear_stop();
    queue.run_until(100);
    CHECK(later_fired);
    CHECK(queue.now() == 100);
}

ARCHLAB_TEST("EventQueue rejects scheduling in the past") {
    EventQueue queue;
    queue.run_until(10);

    CHECK_THROWS_AS(queue.schedule_abs(9, "past", [] {}), std::runtime_error);
}

} // namespace
