#include "archlab/cpu/fake_cpu.h"

#include "archlab/platform/memory_map.h"
#include "archlab/sim/event_queue.h"
#include "archlab/stats/stats_manager.h"

#include <iomanip>
#include <ostream>
#include <stdexcept>
#include <utility>

namespace archlab::cpu {

FakeCpu::FakeCpu(std::string name, sim::EventQueue& event_queue,
                 platform::MemoryMap& memory_map, stats::StatsManager& stats,
                 std::ostream* trace)
    : name_(std::move(name)), event_queue_(event_queue), memory_map_(memory_map), stats_(stats),
      trace_(trace) {
    if (name_.empty()) {
        throw std::invalid_argument("CPU name must not be empty");
    }
}

std::string_view FakeCpu::name() const noexcept {
    return name_;
}

void FakeCpu::alu(std::uint64_t count) {
    stats_.record_instruction(count);
    if (trace_ != nullptr) {
        *trace_ << "[cpu] tick=" << event_queue_.now() << " cpu=" << name_
                << " alu-count=" << count << '\n';
    }
}

void FakeCpu::branch(bool taken) {
    stats_.record_instruction();
    stats_.record_branch();
    if (trace_ != nullptr) {
        *trace_ << "[cpu] tick=" << event_queue_.now() << " cpu=" << name_
                << " branch-taken=" << (taken ? "true" : "false") << '\n';
    }
}

Data FakeCpu::load64(Addr address) {
    stats_.record_instruction();
    stats_.record_load();
    const Data value = memory_map_.read64(address);

    if (trace_ != nullptr) {
        *trace_ << "[cpu] tick=" << event_queue_.now() << " cpu=" << name_
                << " load64-address=0x" << std::hex << address << " value=0x" << value
                << std::dec << '\n';
    }
    return value;
}

void FakeCpu::store64(Addr address, Data value) {
    stats_.record_instruction();
    stats_.record_store();

    if (trace_ != nullptr) {
        *trace_ << "[cpu] tick=" << event_queue_.now() << " cpu=" << name_
                << " store64-address=0x" << std::hex << address << " value=0x" << value
                << std::dec << '\n';
    }
    memory_map_.write64(address, value);
}

} // namespace archlab::cpu
