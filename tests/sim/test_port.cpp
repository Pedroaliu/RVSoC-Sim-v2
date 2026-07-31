#include "archlab/sim/port.h"
#include "test_framework.h"

#include <stdexcept>
#include <string>
#include <utility>

namespace {

using archlab::sim::InputPort;
using archlab::sim::OutputPort;
using archlab::sim::SimObject;

class TestSender final : public SimObject {
public:
    explicit TestSender(std::string name)
        : SimObject(std::move(name)),
          output_(*this, "request_out") {
    }

    [[nodiscard]] OutputPort<int>& output() {
        return output_;
    }

private:
    OutputPort<int> output_;
};

class TestReceiver final : public SimObject {
public:
    explicit TestReceiver(std::string name)
        : SimObject(std::move(name)),
          input_(
              *this,
              "request_in",
              [this](const int& value) {
                  observed_ = value;
              }) {
    }

    [[nodiscard]] InputPort<int>& input() {
        return input_;
    }

    [[nodiscard]] int observed() const {
        return observed_;
    }

private:
    int observed_ = 0;
    InputPort<int> input_;
};

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

ARCHLAB_TEST("OutputPort rejects an empty name") {
    SimObject sender("sender0");

    CHECK_THROWS_AS(OutputPort<int>(sender, ""), std::invalid_argument);
}

ARCHLAB_TEST("OutputPort sends a typed message to a connected InputPort") {
    SimObject sender("sender0");
    SimObject receiver("receiver0");
    int observed = 0;

    InputPort<int> input(
        receiver,
        "request_in",
        [&observed](const int& value) {
            observed = value;
        });
    OutputPort<int> output(sender, "request_out");

    CHECK(!input.is_connected());
    CHECK(!output.is_connected());

    output.connect(input);

    CHECK(input.is_connected());
    CHECK(output.is_connected());

    output.send(42);

    CHECK(observed == 42);
    CHECK(output.owner().name() == "sender0");
    CHECK(output.name() == "request_out");
}

ARCHLAB_TEST("OutputPort rejects sending before connection") {
    SimObject sender("sender0");
    OutputPort<int> output(sender, "request_out");

    CHECK_THROWS_AS(output.send(42), std::logic_error);
    CHECK(!output.is_connected());
}

ARCHLAB_TEST("OutputPort rejects a second connection") {
    SimObject sender("sender0");
    SimObject receiver0("receiver0");
    SimObject receiver1("receiver1");

    InputPort<int> input0(
        receiver0,
        "request_in",
        [](const int&) {
        });
    InputPort<int> input1(
        receiver1,
        "request_in",
        [](const int&) {
        });
    OutputPort<int> output(sender, "request_out");

    output.connect(input0);

    CHECK_THROWS_AS(output.connect(input1), std::logic_error);

    CHECK(output.is_connected());
    CHECK(input0.is_connected());
    CHECK(!input1.is_connected());
}

ARCHLAB_TEST("InputPort rejects multiple output connections") {
    SimObject sender0("sender0");
    SimObject sender1("sender1");
    SimObject receiver("receiver0");

    InputPort<int> input(
        receiver,
        "request_in",
        [](const int&) {
        });
    OutputPort<int> output0(sender0, "request_out");
    OutputPort<int> output1(sender1, "request_out");

    output0.connect(input);

    CHECK_THROWS_AS(output1.connect(input), std::logic_error);

    CHECK(output0.is_connected());
    CHECK(!output1.is_connected());
    CHECK(input.is_connected());
}

ARCHLAB_TEST("Ports embedded in SimObject components communicate") {
    TestSender sender("sender0");
    TestReceiver receiver("receiver0");

    sender.output().connect(receiver.input());
    sender.output().send(42);

    CHECK(receiver.observed() == 42);
    CHECK(sender.output().owner().name() == "sender0");
    CHECK(receiver.input().owner().name() == "receiver0");
}

} // namespace
