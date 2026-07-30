#include "archlab/devices/simple_ram.h"
#include "archlab/platform/mmio_device.h"
#include "test_framework.h"

#include <stdexcept>

namespace {

using archlab::devices::SimpleRam;
using archlab::platform::AccessSize;

ARCHLAB_TEST("SimpleRam reads and writes little-endian data") {
    SimpleRam ram("dram0", 1024);
    ram.write(0, AccessSize::Double, 0x1122334455667788ULL);

    CHECK(ram.read(0, AccessSize::Double) == 0x1122334455667788ULL);
    CHECK(ram.read(0, AccessSize::Byte) == 0x88);
    CHECK(ram.read(7, AccessSize::Byte) == 0x11);
}

ARCHLAB_TEST("SimpleRam rejects out-of-range accesses") {
    SimpleRam ram("dram0", 1024);

    CHECK_THROWS_AS(ram.write(1020, AccessSize::Double, 0), std::out_of_range);
}

} // namespace
