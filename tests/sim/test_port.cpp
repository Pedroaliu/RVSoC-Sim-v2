#include "archlab/sim/port.h"
#include "test_framework.h"

#include <stdexcept>
#include <string>
#include <utility>

namespace {

using archlab::sim::InputPort;
using archlab::sim::Link;
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

ARCHLAB_TEST("Link rejects an empty name") {
    CHECK_THROWS_AS(Link<int>(""), std::invalid_argument);
}

ARCHLAB_TEST("Typed Link connects ports and delivers a message") {
    SimObject sender("sender0");
    SimObject receiver("receiver0");
    int observed = 0;

    OutputPort<int> output(sender, "request_out");
    InputPort<int> input(
        receiver,
        "request_in",
        [&observed](const int& value) {
            observed = value;
        });
    Link<int> link("request_link");

    CHECK(!output.is_connected());
    CHECK(!input.is_connected());
    CHECK(!link.is_connected());

    link.connect(output, input);

    CHECK(output.is_connected());
    CHECK(input.is_connected());
    CHECK(link.is_connected());
    CHECK(link.source().owner().name() == "sender0");
    CHECK(link.source().name() == "request_out");
    CHECK(link.sink().owner().name() == "receiver0");
    CHECK(link.sink().name() == "request_in");

    output.send(42);

    CHECK(observed == 42);
}

ARCHLAB_TEST("OutputPort rejects sending before link connection") {
    SimObject sender("sender0");
    OutputPort<int> output(sender, "request_out");

    CHECK_THROWS_AS(output.send(42), std::logic_error);
    CHECK(!output.is_connected());
}

ARCHLAB_TEST("Unconnected Link rejects endpoint inspection") {
    Link<int> link("request_link");

    CHECK_THROWS_AS((void)link.source(), std::logic_error);
    CHECK_THROWS_AS((void)link.sink(), std::logic_error);
}

ARCHLAB_TEST("Link rejects a second connection") {
    SimObject sender0("sender0");
    SimObject sender1("sender1");
    SimObject receiver0("receiver0");
    SimObject receiver1("receiver1");

    OutputPort<int> output0(sender0, "request_out");
    OutputPort<int> output1(sender1, "request_out");
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
    Link<int> link("request_link");

    link.connect(output0, input0);

    CHECK_THROWS_AS(link.connect(output1, input1), std::logic_error);
    CHECK(link.source().owner().name() == "sender0");
    CHECK(link.sink().owner().name() == "receiver0");
    CHECK(!output1.is_connected());
    CHECK(!input1.is_connected());
}

ARCHLAB_TEST("OutputPort rejects multiple Link connections") {
    SimObject sender("sender0");
    SimObject receiver0("receiver0");
    SimObject receiver1("receiver1");

    OutputPort<int> output(sender, "request_out");
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
    Link<int> link0("link0");
    Link<int> link1("link1");

    link0.connect(output, input0);

    CHECK_THROWS_AS(link1.connect(output, input1), std::logic_error);
    CHECK(link0.is_connected());
    CHECK(!link1.is_connected());
    CHECK(!input1.is_connected());
}

ARCHLAB_TEST("InputPort rejects multiple Link connections without partial binding") {
    SimObject sender0("sender0");
    SimObject sender1("sender1");
    SimObject receiver("receiver0");

    OutputPort<int> output0(sender0, "request_out");
    OutputPort<int> output1(sender1, "request_out");
    InputPort<int> input(
        receiver,
        "request_in",
        [](const int&) {
        });
    Link<int> link0("link0");
    Link<int> link1("link1");

    link0.connect(output0, input);

    CHECK_THROWS_AS(link1.connect(output1, input), std::logic_error);
    CHECK(link0.is_connected());
    CHECK(!link1.is_connected());
    CHECK(!output1.is_connected());
}

ARCHLAB_TEST("Ports embedded in SimObject components communicate through a Link") {
    TestSender sender("sender0");
    TestReceiver receiver("receiver0");
    Link<int> link("request_link");

    link.connect(sender.output(), receiver.input());
    sender.output().send(42);

    CHECK(receiver.observed() == 42);
    CHECK(sender.output().owner().name() == "sender0");
    CHECK(receiver.input().owner().name() == "receiver0");
    CHECK(link.name() == "request_link");
}

} // namespace
