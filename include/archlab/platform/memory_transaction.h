//
// Created by pedroa on 2026-08-01.
//

#ifndef ARCHLAB_PLATFORM_MEMORY_TRANSACTION_H
#define ARCHLAB_PLATFORM_MEMORY_TRANSACTION_H

#include "archlab/base/types.h"
#include "archlab/platform/mmio_device.h"
#include "archlab/sim/transaction_id.h"

#include <cstdint>

namespace archlab::platform {

enum class MemoryCommand : std::uint8_t {
    Read,
    Write,
};

struct MemRequest {
    sim::TransactionKey key{};
    MemoryCommand command{MemoryCommand::Read};
    Addr address{};
    AccessSize size{AccessSize::Byte};
    Data data{};

    bool operator==(const MemRequest&) const = default;
};

struct MemResponse {
    sim::TransactionKey key{};
    Data data{};

    bool operator==(const MemResponse&) const = default;
};

} // namespace archlab::platform

#endif // ARCHLAB_PLATFORM_MEMORY_TRANSACTION_H