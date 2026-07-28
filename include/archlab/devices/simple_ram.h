#ifndef ARCHLAB_DEVICES_SIMPLE_RAM_H
#define ARCHLAB_DEVICES_SIMPLE_RAM_H

#include "archlab/platform/mmio_device.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace archlab::devices {

class SimpleRam final : public platform::MmioDevice {
public:
    SimpleRam(std::string name, std::size_t size);

    [[nodiscard]] std::string_view name() const noexcept override;
    [[nodiscard]] Data read(Addr offset, platform::AccessSize size) override;
    void write(Addr offset, platform::AccessSize size, Data value) override;

    [[nodiscard]] std::size_t size() const noexcept;

private:
    void check_range(Addr offset, platform::AccessSize size) const;

    std::string name_;
    std::vector<std::uint8_t> storage_;
};

} // namespace archlab::devices

#endif // ARCHLAB_DEVICES_SIMPLE_RAM_H
