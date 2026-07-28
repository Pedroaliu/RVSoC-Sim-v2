#include "archlab/cpu/fake_cpu.h"
#include "archlab/devices/magic_device.h"
#include "archlab/devices/simple_ram.h"
#include "archlab/platform/memory_map.h"
#include "archlab/sim/event_queue.h"
#include "archlab/stats/stats_manager.h"

#include <iomanip>
#include <iostream>

int main() {
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

    constexpr Addr dram_base = 0x80000000ULL;
    constexpr Addr magic_base = 0x40000000ULL;

    EventQueue event_queue;
    event_queue.set_trace_stream(&std::cout);

    StatsManager stats;
    MemoryMap address_space;
    SimpleRam ram("dram0", 4096);
    MagicDevice magic(event_queue, "magic0", &stats, &std::cout);
    FakeCpu cpu0("cpu0", event_queue, address_space, stats, &std::cout);

    address_space.add_region(dram_base, ram.size(), ram);
    address_space.add_region(magic_base, 4096, magic);

    event_queue.schedule_abs(100, "reset stats", [&] {
        address_space.write64(magic_base, encode(MagicCommand::ResetStats));
    });
    event_queue.schedule_abs(120, "begin ROI", [&] {
        address_space.write64(magic_base, encode(MagicCommand::BeginRoi));
    });
    event_queue.schedule_abs(130, "workload phase 1", [&] {
        cpu0.alu(9);
        cpu0.store64(dram_base, 0x1122334455667788ULL);
    });
    event_queue.schedule_abs(160, "workload phase 2", [&] {
        cpu0.alu(16);
        const Data value = cpu0.load64(dram_base);
        cpu0.branch(true);
        cpu0.branch(false);
        cpu0.branch(true);
        std::cout << "[demo] loaded=0x" << std::hex << value << std::dec << '\n';
    });
    event_queue.schedule_abs(190, "end ROI", [&] {
        address_space.write64(magic_base, encode(MagicCommand::EndRoi));
    });
    event_queue.schedule_abs(200, "dump stats", [&] {
        address_space.write64(magic_base, encode(MagicCommand::DumpStats));
    });

    event_queue.run_until(250);
    std::cout << "Simulation finished at tick " << event_queue.now() << '\n';
    return 0;
}
