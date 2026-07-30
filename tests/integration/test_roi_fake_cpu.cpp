#include "archlab/base/types.h"
#include "archlab/cpu/fake_cpu.h"
#include "archlab/devices/magic_device.h"
#include "archlab/devices/simple_ram.h"
#include "archlab/platform/memory_map.h"
#include "archlab/sim/event_queue.h"
#include "archlab/stats/stats_manager.h"
#include "test_framework.h"

namespace {

using archlab::Addr;
using archlab::Data;
using archlab::cpu::FakeCpu;
using archlab::devices::MagicCommand;
using archlab::devices::MagicDevice;
using archlab::devices::SimpleRam;
using archlab::devices::encode;
using archlab::platform::MemoryMap;
using archlab::sim::EventQueue;
using archlab::stats::StatsManager;

ARCHLAB_TEST("ROI magic controls FakeCpu statistics") {
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

} // namespace
