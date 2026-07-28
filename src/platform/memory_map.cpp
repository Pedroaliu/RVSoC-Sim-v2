#include "archlab/platform/memory_map.h"

#include <algorithm>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>

namespace archlab::platform {

void MemoryMap::add_region(Addr base, std::uint64_t size, MmioDevice& device) {
    if (size == 0) {
        throw std::invalid_argument("memory region size must be greater than zero");
    }

    const Addr end = checked_end(base, size, std::string(device.name()));
    const auto position = std::lower_bound(
        regions_.begin(), regions_.end(), base,
        [](const Region& region, Addr candidate_base) { return region.base < candidate_base; });

    if (position != regions_.end() && end > position->base) {
        throw std::runtime_error("memory region overlaps " + std::string(position->device->name()));
    }
    if (position != regions_.begin()) {
        const Region& previous = *std::prev(position);
        if (base < previous.end) {
            throw std::runtime_error("memory region overlaps " +
                                     std::string(previous.device->name()));
        }
    }

    regions_.insert(position, Region{.base = base, .end = end, .device = &device});
}

Data MemoryMap::read(Addr address, AccessSize size) {
    const LookupResult result = find_region(address, size);
    return result.region->device->read(result.offset, size);
}

void MemoryMap::write(Addr address, AccessSize size, Data value) {
    const LookupResult result = find_region(address, size);
    result.region->device->write(result.offset, size, value);
}

Data MemoryMap::read64(Addr address) {
    return read(address, AccessSize::Double);
}

void MemoryMap::write64(Addr address, Data value) {
    write(address, AccessSize::Double, value);
}

std::size_t MemoryMap::region_count() const noexcept {
    return regions_.size();
}

MemoryMap::LookupResult MemoryMap::find_region(Addr address, AccessSize size) {
    const Addr access_end = checked_end(address, bytes(size), "memory access");

    const auto position = std::upper_bound(
        regions_.begin(), regions_.end(), address,
        [](Addr candidate_address, const Region& region) { return candidate_address < region.base; });

    if (position == regions_.begin()) {
        throw std::runtime_error("unmapped memory access at 0x" + hex(address));
    }

    Region& region = *std::prev(position);
    if (address < region.base || access_end > region.end) {
        throw std::runtime_error("unmapped or cross-region memory access at 0x" + hex(address));
    }

    return LookupResult{.region = &region, .offset = address - region.base};
}

Addr MemoryMap::checked_end(Addr base, std::uint64_t size, const std::string& context) {
    if (size > std::numeric_limits<Addr>::max() - base) {
        throw std::overflow_error(context + " exceeds address range");
    }
    return base + size;
}

std::string MemoryMap::hex(Addr value) {
    std::ostringstream stream;
    stream << std::hex << value;
    return stream.str();
}

} // namespace archlab::platform
