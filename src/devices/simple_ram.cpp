#include "archlab/devices/simple_ram.h"

#include <stdexcept>
#include <utility>

namespace archlab::devices {

SimpleRam::SimpleRam(std::string name, std::size_t size)
    : name_(std::move(name)), storage_(size, 0) {
    if (name_.empty()) {
        throw std::invalid_argument("RAM name must not be empty");
    }
    if (size == 0) {
        throw std::invalid_argument("RAM size must be greater than zero");
    }
}

std::string_view SimpleRam::name() const noexcept {
    return name_;
}

Data SimpleRam::read(Addr offset, platform::AccessSize size) {
    check_range(offset, size);

    Data value = 0;
    const std::uint8_t width = platform::bytes(size);
    for (std::uint8_t index = 0; index < width; ++index) {
        value |= static_cast<Data>(storage_[static_cast<std::size_t>(offset) + index])
                 << (8U * index);
    }
    return value;
}

void SimpleRam::write(Addr offset, platform::AccessSize size, Data value) {
    check_range(offset, size);

    const std::uint8_t width = platform::bytes(size);
    for (std::uint8_t index = 0; index < width; ++index) {
        storage_[static_cast<std::size_t>(offset) + index] =
            static_cast<std::uint8_t>((value >> (8U * index)) & 0xffU);
    }
}

std::size_t SimpleRam::size() const noexcept {
    return storage_.size();
}

void SimpleRam::check_range(Addr offset, platform::AccessSize size) const {
    const std::uint64_t width = platform::bytes(size);
    const std::uint64_t storage_size = storage_.size();
    if (offset > storage_size || width > storage_size - offset) {
        throw std::out_of_range("RAM access out of range in " + name_);
    }
}

} // namespace archlab::devices
