#ifndef ARCHLAB_PLATFORM_MMIO_DEVICE_H
#define ARCHLAB_PLATFORM_MMIO_DEVICE_H

#include "archlab/base/types.h"

#include <cstdint>
#include <string_view>

namespace archlab::platform {

enum class AccessSize : std::uint8_t {
    Byte = 1,
    Half = 2,
    Word = 4,
    Double = 8,
};

constexpr std::uint8_t bytes(AccessSize size) noexcept {
    return static_cast<std::uint8_t>(size);
}

class MmioDevice {
public:
    virtual ~MmioDevice() = default;

    [[nodiscard]] virtual std::string_view name() const noexcept = 0;
    [[nodiscard]] virtual Data read(Addr offset, AccessSize size) = 0;
    virtual void write(Addr offset, AccessSize size, Data value) = 0;
};

} // namespace archlab::platform

#endif // ARCHLAB_PLATFORM_MMIO_DEVICE_H
