#ifndef ARCHLAB_CPU_FAKE_CPU_H
#define ARCHLAB_CPU_FAKE_CPU_H

#include "archlab/base/types.h"

#include <cstdint>
#include <iosfwd>
#include <string>
#include <string_view>

namespace archlab::platform {
class MemoryMap;
}

namespace archlab::sim {
class EventQueue;
}

namespace archlab::stats {
class StatsManager;
}

namespace archlab::cpu {

class FakeCpu {
public:
    FakeCpu(std::string name, sim::EventQueue& event_queue, platform::MemoryMap& memory_map,
            stats::StatsManager& stats, std::ostream* trace = nullptr);

    [[nodiscard]] std::string_view name() const noexcept;

    void alu(std::uint64_t count = 1);
    void branch(bool taken);
    [[nodiscard]] Data load64(Addr address);
    void store64(Addr address, Data value);

private:
    std::string name_;
    sim::EventQueue& event_queue_;
    platform::MemoryMap& memory_map_;
    stats::StatsManager& stats_;
    std::ostream* trace_;
};

} // namespace archlab::cpu

#endif // ARCHLAB_CPU_FAKE_CPU_H
