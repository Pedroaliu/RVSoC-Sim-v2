#include "archlab/base/types.h"
#include "archlab/devices/simple_ram.h"
#include "archlab/platform/memory_map.h"
#include "test_framework.h"

#include <stdexcept>

namespace {

using archlab::Addr;
using archlab::devices::SimpleRam;
using archlab::platform::MemoryMap;

ARCHLAB_TEST("MemoryMap routes accesses and reports region count") {
    MemoryMap map;
    SimpleRam ram("dram0", 4096);
    constexpr Addr base = 0x80000000ULL;

    map.add_region(base, ram.size(), ram);
    map.write64(base + 8, 0x123456789abcdef0ULL);

    CHECK(map.read64(base + 8) == 0x123456789abcdef0ULL);
    CHECK(map.region_count() == 1);
}

ARCHLAB_TEST("MemoryMap rejects overlapping regions") {
    MemoryMap map;
    SimpleRam ram0("dram0", 4096);
    SimpleRam ram1("dram1", 4096);
    constexpr Addr base = 0x80000000ULL;

    map.add_region(base, ram0.size(), ram0);
    CHECK_THROWS_AS(map.add_region(base + 2048, ram1.size(), ram1), std::runtime_error);
}

ARCHLAB_TEST("MemoryMap rejects unmapped accesses") {
    MemoryMap map;
    SimpleRam ram("dram0", 4096);
    constexpr Addr base = 0x80000000ULL;

    map.add_region(base, ram.size(), ram);
    CHECK_THROWS_AS(map.read64(0x90000000ULL), std::runtime_error);
}

} // namespace
