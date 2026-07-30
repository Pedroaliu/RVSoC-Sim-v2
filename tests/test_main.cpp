#include "archlab/cpu/fake_cpu.h"
#include "archlab/devices/magic_device.h"
#include "archlab/devices/simple_ram.h"
#include "archlab/platform/memory_map.h"
#include "archlab/sim/clock_domain.h"
#include "archlab/sim/event_queue.h"
#include "archlab/sim/time.h"
#include "archlab/stats/stats_manager.h"
#include "archlab/sim/sim_object.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>


#define CHECK(...)                                                                                \
    do {                                                                                          \
        if (!(__VA_ARGS__)) {                                                                     \
            std::cerr << "CHECK failed: " << #__VA_ARGS__ << " at " << __FILE__ << ':'          \
                      << __LINE__ << '\n';                                                       \
            std::exit(EXIT_FAILURE);                                                              \
        }                                                                                         \
    } while (false)

namespace {

using archlab::Addr;
using archlab::Data;
using archlab::cpu::FakeCpu;
using archlab::devices::MagicCommand;
using archlab::devices::MagicDevice;
using archlab::devices::SimpleRam;
using archlab::devices::encode;
using archlab::platform::AccessSize;
using archlab::platform::MemoryMap;
using archlab::sim::ClockDomain;
using archlab::sim::EventQueue;
using archlab::sim::TimePoint;
using archlab::sim::at_tick;
using archlab::sim::next_delta;
using archlab::sim::next_phase;
using archlab::stats::StatsManager;
using archlab::sim::LifecycleState;
using archlab::sim::SimObject;

class TestSimObject final : public SimObject {
public:
    explicit TestSimObject(std::string name)
        : SimObject(std::move(name)) {

    }

    [[nodiscard]] int initialize_count() const {
        return initialize_count_;
    }

    [[nodiscard]] int reset_count() const {
        return reset_count_;
    }

    [[nodiscard]] int startup_count() const {
        return startup_count_;
    }

protected:
    void on_initialize() override {
        ++initialize_count_;
    }

    void on_reset() override {
        ++reset_count_;
    }

    void on_startup() override {
        ++startup_count_;
    }
private:
    int initialize_count_ = 0;
    int reset_count_ = 0;
    int startup_count_ = 0;
};

void test_time_ordering() {
    CHECK(at_tick(9) < at_tick(10));
    CHECK(TimePoint{.tick = 10, .phase = 0, .delta = 7} <
          TimePoint{.tick = 10, .phase = 1, .delta = 0});
    CHECK(next_delta(at_tick(10)) == TimePoint{.tick = 10, .phase = 0, .delta = 1});
    CHECK(next_phase(at_tick(10)) == TimePoint{.tick = 10, .phase = 1, .delta = 0});
}

void test_clock_domain() {
    const ClockDomain clock(3, 10);
    CHECK(clock.edge(0) == 3);
    CHECK(clock.edge(4) == 43);
    CHECK(clock.is_edge(23));
    CHECK(!clock.is_edge(24));
    CHECK(clock.next_edge_at_or_after(24) == 33);
    CHECK(clock.cycle_at_or_before(32).value() == 2);

    bool caught = false;
    try {
        const ClockDomain invalid(0, 0);
        (void)invalid;
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    CHECK(caught);
}

void test_event_queue_order() {
    EventQueue queue;
    std::vector<int> order;

    queue.schedule_abs(30, "event30", [&] { order.push_back(30); });
    queue.schedule_abs(10, "event10", [&] { order.push_back(10); });
    queue.schedule_abs(20, "event20", [&] { order.push_back(20); });
    queue.run_until(100);

    CHECK((order == std::vector<int>{10, 20, 30}));
    CHECK(queue.now() == 100);
}

void test_event_queue_phase_delta_and_sequence_order() {
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

void test_event_scheduled_at_current_time_is_stable() {
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

void test_event_queue_cancel_and_stop() {
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

void test_event_queue_rejects_past() {
    EventQueue queue;
    queue.run_until(10);

    bool caught = false;
    try {
        queue.schedule_abs(9, "past", [] {});
    } catch (const std::runtime_error&) {
        caught = true;
    }
    CHECK(caught);
}

void test_simple_ram() {
    SimpleRam ram("dram0", 1024);
    ram.write(0, AccessSize::Double, 0x1122334455667788ULL);

    CHECK(ram.read(0, AccessSize::Double) == 0x1122334455667788ULL);
    CHECK(ram.read(0, AccessSize::Byte) == 0x88);
    CHECK(ram.read(7, AccessSize::Byte) == 0x11);

    bool caught = false;
    try {
        ram.write(1020, AccessSize::Double, 0);
    } catch (const std::out_of_range&) {
        caught = true;
    }
    CHECK(caught);
}

void test_memory_map() {
    MemoryMap map;
    SimpleRam ram0("dram0", 4096);
    SimpleRam ram1("dram1", 4096);
    constexpr Addr base = 0x80000000ULL;

    map.add_region(base, ram0.size(), ram0);
    map.write64(base + 8, 0x123456789abcdef0ULL);
    CHECK(map.read64(base + 8) == 0x123456789abcdef0ULL);
    CHECK(map.region_count() == 1);

    bool overlap = false;
    try {
        map.add_region(base + 2048, ram1.size(), ram1);
    } catch (const std::runtime_error&) {
        overlap = true;
    }
    CHECK(overlap);

    bool unmapped = false;
    try {
        (void)map.read64(0x90000000ULL);
    } catch (const std::runtime_error&) {
        unmapped = true;
    }
    CHECK(unmapped);
}

void test_roi_magic_and_fake_cpu() {
    EventQueue queue;
    StatsManager stats;
    MemoryMap map;
    SimpleRam ram("dram0", 4096);
    MagicDevice magic(queue, "magic0", &stats);
    constexpr Addr dram_base = 0x80000000ULL;
    constexpr Addr magic_base = 0x40000000ULL;

    map.add_region(dram_base, ram.size(), ram);
    map.add_region(magic_base, 4096, magic);
    FakeCpu cpu("cpu0", queue, map, stats);

    cpu.alu(5);
    CHECK(stats.snapshot(queue.now()).instructions == 0);

    map.write64(magic_base, encode(MagicCommand::BeginRoi));
    cpu.alu(10);
    cpu.store64(dram_base, 0x1122334455667788ULL);
    const Data value = cpu.load64(dram_base);
    cpu.branch(true);
    queue.run_until(10);
    map.write64(magic_base, encode(MagicCommand::EndRoi));

    const auto snapshot = stats.snapshot(queue.now());
    CHECK(value == 0x1122334455667788ULL);
    CHECK(snapshot.roi_ticks == 10);
    CHECK(snapshot.instructions == 13);
    CHECK(snapshot.loads == 1);
    CHECK(snapshot.stores == 1);
    CHECK(snapshot.branches == 1);
}

void test_sim_object_identity() {
    const SimObject object("timer0");

    CHECK(object.name() == "timer0");
    CHECK(object.state() == LifecycleState::Constructed);
}

void test_sim_object_lifecycle() {
    TestSimObject object("timer0");

    CHECK(object.state() == LifecycleState::Constructed);

    object.initialize();
    CHECK(object.state() == LifecycleState::Initialized);
    CHECK(object.initialize_count() == 1);

    object.reset();
    CHECK(object.state() == LifecycleState::Ready);
    CHECK(object.reset_count() == 1);

    object.startup();
    CHECK(object.state() == LifecycleState::Running);
    CHECK(object.startup_count() == 1);
}

void test_sim_object_rejects_invalid_transition() {
    TestSimObject object("timer0");

    bool caught = false;

    try {
        object.startup();
    } catch (const std::logic_error&) {
        caught = true;
    }

    CHECK(caught);
    CHECK(object.state() == LifecycleState::Constructed);
    CHECK(object.startup_count() == 0);
}

} // namespace

int main() {
    test_time_ordering();
    test_clock_domain();
    test_event_queue_order();
    test_event_queue_phase_delta_and_sequence_order();
    test_event_scheduled_at_current_time_is_stable();
    test_event_queue_cancel_and_stop();
    test_event_queue_rejects_past();
    test_simple_ram();
    test_memory_map();
    test_roi_magic_and_fake_cpu();
    test_sim_object_identity();
    test_sim_object_lifecycle();
    test_sim_object_rejects_invalid_transition();
    std::cout << "All RVSoC-Sim v2 baseline tests passed.\n";
    return EXIT_SUCCESS;
}
