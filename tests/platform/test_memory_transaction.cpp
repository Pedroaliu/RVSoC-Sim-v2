//
// Created by pedroa on 2026-08-01.
//
#include "archlab/base/types.h"
#include "archlab/devices/simple_ram.h"
#include "archlab/platform/memory_map.h"
#include "archlab/platform/memory_transaction.h"
#include "test_framework.h"

#include <stdexcept>

namespace {

using archlab::Addr;
using archlab::Data;
using archlab::platform::AccessSize;
using archlab::platform::MemoryCommand;
using archlab::platform::MemRequest;
using archlab::platform::MemResponse;
using archlab::sim::RequestId;
using archlab::sim::SourceId;
using archlab::sim::TransactionKey;

ARCHLAB_TEST("Memory transaction request and response equality") {
    MemRequest req1{
        .key = TransactionKey{.source_id = SourceId{1}, .request_id = RequestId{42}},
        .command = MemoryCommand::Read,
        .address = 0x1000,
        .size = AccessSize::Word,
        .data = 0xdeadbeef,
    };

    MemRequest req2{
        .key = TransactionKey{.source_id = SourceId{1}, .request_id = RequestId{42}},
        .command = MemoryCommand::Read,
        .address = 0x1000,
        .size = AccessSize::Word,
        .data = 0xdeadbeef,
    };

    MemResponse resp1{
        .key = TransactionKey{.source_id = SourceId{1}, .request_id = RequestId{42}},
        .data = 0xdeadbeef,
    };

    MemResponse resp2{
        .key = TransactionKey{.source_id = SourceId{1}, .request_id = RequestId{42}},
        .data = 0xdeadbeef,
    };

    CHECK(req1 == req2);
    CHECK(resp1 == resp2);
}

ARCHLAB_TEST("MemRequest represents a typed write transaction") {
    const TransactionKey key{
        .source_id = SourceId{3},
        .request_id = RequestId{8},
    };

    const MemRequest request{
        .key = key,
        .command = MemoryCommand::Write,
        .address = Addr{0x80000008},
        .size = AccessSize::Double,
        .data = Data{0x1122334455667788ULL},
    };

    CHECK(request.key == key);
    CHECK(request.command == MemoryCommand::Write);
    CHECK(request.address == Addr{0x80000008});
    CHECK(request.size == AccessSize::Double);
    CHECK(request.data == Data{0x1122334455667788ULL});
}

ARCHLAB_TEST("MemResponse preserves transaction identity and returned data") {
    const TransactionKey key{
        .source_id = SourceId{4},
        .request_id = RequestId{11},
    };

    const MemResponse response{
        .key = key,
        .data = Data{0x123456789abcdef0ULL},
    };

    CHECK(response.key == key);
    CHECK(response.data == Data{0x123456789abcdef0ULL});
}

} // namespace