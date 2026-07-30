#include "archlab/sim/port.h"
#include "test_framework.h"

#include <stdexcept>

namespace {

using archlab::sim::InputPort;
using archlab::sim::SimObject;

ARCHLAB_TEST("InputPort delivers a typed message to its handler") {
    SimObject receiver("receiver0");
    int observed = 0;

    InputPort<int> input(
        receiver,
        "request_in",
        [&observed](const int& value) {
            observed = value;
        });

    input.receive(42);

    CHECK(observed == 42);
    CHECK(input.name() == "request_in");
    CHECK(input.owner().name() == "receiver0");
}

ARCHLAB_TEST("InputPort rejects an empty name") {
    SimObject receiver("receiver0");

    CHECK_THROWS_AS(
        InputPort<int>(
            receiver,
            "",
            [](const int&) {
            }),
        std::invalid_argument);
}

ARCHLAB_TEST("InputPort rejects an empty handler") {
    SimObject receiver("receiver0");
    InputPort<int>::Handler empty_handler;

    CHECK_THROWS_AS(
        InputPort<int>(receiver, "request_in", empty_handler),
        std::invalid_argument);
}

} // namespace
