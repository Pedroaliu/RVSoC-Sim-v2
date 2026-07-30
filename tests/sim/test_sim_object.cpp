#include "archlab/sim/sim_object.h"
#include "test_framework.h"

#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace {

using archlab::sim::LifecycleState;
using archlab::sim::SimObject;

static_assert(!std::is_copy_constructible_v<SimObject>);
static_assert(!std::is_copy_assignable_v<SimObject>);
static_assert(!std::is_move_constructible_v<SimObject>);
static_assert(!std::is_move_assignable_v<SimObject>);

class TestSimObject final : public SimObject {
public:
    explicit TestSimObject(std::string name)
        : SimObject(std::move(name)) {
    }

    [[nodiscard]] int initialize_count() const {
        return initialize_count_;
    }

    [[nodiscard]] int reset_count() const {
        return reset_count_;
    }

    [[nodiscard]] int startup_count() const {
        return startup_count_;
    }

protected:
    void on_initialize() override {
        ++initialize_count_;
    }

    void on_reset() override {
        ++reset_count_;
    }

    void on_startup() override {
        ++startup_count_;
    }

private:
    int initialize_count_ = 0;
    int reset_count_ = 0;
    int startup_count_ = 0;
};

ARCHLAB_TEST("SimObject keeps a stable identity and initial state") {
    const SimObject object("timer0");

    CHECK(object.name() == "timer0");
    CHECK(object.state() == LifecycleState::Constructed);
}

ARCHLAB_TEST("SimObject rejects an empty name") {
    CHECK_THROWS_AS(SimObject(""), std::invalid_argument);
}

ARCHLAB_TEST("SimObject follows the minimal lifecycle") {
    TestSimObject object("timer0");

    CHECK(object.state() == LifecycleState::Constructed);

    object.initialize();
    CHECK(object.state() == LifecycleState::Initialized);
    CHECK(object.initialize_count() == 1);

    object.reset();
    CHECK(object.state() == LifecycleState::Ready);
    CHECK(object.reset_count() == 1);

    object.startup();
    CHECK(object.state() == LifecycleState::Running);
    CHECK(object.startup_count() == 1);
}

ARCHLAB_TEST("SimObject rejects startup before reset") {
    TestSimObject object("timer0");

    CHECK_THROWS_AS(object.startup(), std::logic_error);
    CHECK(object.state() == LifecycleState::Constructed);
    CHECK(object.startup_count() == 0);
}

ARCHLAB_TEST("SimObject rejects repeated initialization before calling the hook") {
    TestSimObject object("timer0");

    object.initialize();
    CHECK_THROWS_AS(object.initialize(), std::logic_error);

    CHECK(object.state() == LifecycleState::Initialized);
    CHECK(object.initialize_count() == 1);
}

} // namespace
