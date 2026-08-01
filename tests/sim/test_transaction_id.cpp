#include "archlab/sim/transaction_id.h"
#include "test_framework.h"

namespace {

using archlab::sim::RequestId;
using archlab::sim::SourceId;
using archlab::sim::TransactionKey;

ARCHLAB_TEST("TransactionKey compares equal only when both identity fields match") {
    const TransactionKey first{
        .source_id = SourceId{3},
        .request_id = RequestId{7},
    };
    const TransactionKey same{
        .source_id = SourceId{3},
        .request_id = RequestId{7},
    };

    CHECK(first == same);
}

ARCHLAB_TEST("TransactionKey distinguishes different sources") {
    const TransactionKey first{
        .source_id = SourceId{3},
        .request_id = RequestId{7},
    };
    const TransactionKey second{
        .source_id = SourceId{4},
        .request_id = RequestId{7},
    };

    CHECK(first != second);
}

ARCHLAB_TEST("TransactionKey distinguishes different requests from one source") {
    const TransactionKey first{
        .source_id = SourceId{3},
        .request_id = RequestId{7},
    };
    const TransactionKey second{
        .source_id = SourceId{3},
        .request_id = RequestId{8},
    };

    CHECK(first != second);
}

ARCHLAB_TEST("TransactionKey orders by source before request") {
    const TransactionKey earlier_source{
        .source_id = SourceId{2},
        .request_id = RequestId{100},
    };
    const TransactionKey later_source{
        .source_id = SourceId{3},
        .request_id = RequestId{1},
    };

    CHECK(earlier_source < later_source);
}

ARCHLAB_TEST("TransactionKey orders requests within one source") {
    const TransactionKey earlier_request{
        .source_id = SourceId{3},
        .request_id = RequestId{7},
    };
    const TransactionKey later_request{
        .source_id = SourceId{3},
        .request_id = RequestId{8},
    };

    CHECK(earlier_request < later_request);
}

ARCHLAB_TEST("Zero-valued transaction identity fields are valid") {
    const TransactionKey zero{};
    const TransactionKey explicit_zero{
        .source_id = SourceId{0},
        .request_id = RequestId{0},
    };

    CHECK(zero == explicit_zero);
}

} // namespace
