#ifndef ARCHLAB_PLATFORM_MEMORY_MAP_H
#define ARCHLAB_PLATFORM_MEMORY_MAP_H

#include "archlab/platform/mmio_device.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace archlab::platform {

class MemoryMap {
public:
    void add_region(Addr base, std::uint64_t size, MmioDevice& device);

    [[nodiscard]] Data read(Addr address, AccessSize size);
    void write(Addr address, AccessSize size, Data value);

    [[nodiscard]] Data read64(Addr address);
    void write64(Addr address, Data value);

    [[nodiscard]] std::size_t region_count() const noexcept;

private:
    struct Region {
        Addr base = 0;
        Addr end = 0;
        MmioDevice* device = nullptr;
    };

    struct LookupResult {
        Region* region = nullptr;
        Addr offset = 0;
    };

    [[nodiscard]] LookupResult find_region(Addr address, AccessSize size);
    [[nodiscard]] static Addr checked_end(Addr base, std::uint64_t size,
                                          const std::string& context);
    [[nodiscard]] static std::string hex(Addr value);

    std::vector<Region> regions_;
};

} // namespace archlab::platform

#endif // ARCHLAB_PLATFORM_MEMORY_MAP_H
