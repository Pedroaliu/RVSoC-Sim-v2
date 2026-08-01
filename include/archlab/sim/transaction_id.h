#ifndef ARCHLAB_SIM_TRANSACTION_ID_H
#define ARCHLAB_SIM_TRANSACTION_ID_H

#include <compare>
#include <cstdint>

namespace archlab::sim {

struct SourceId {
    std::uint32_t value{};

    auto operator<=>(const SourceId&) const = default;
};

struct RequestId {
    std::uint64_t value{};

    auto operator<=>(const RequestId&) const = default;
};

struct TransactionKey {
    SourceId source_id{};
    RequestId request_id{};

    auto operator<=>(const TransactionKey&) const = default;
};

} // namespace archlab::sim

#endif // ARCHLAB_SIM_TRANSACTION_ID_H
