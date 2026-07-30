#ifndef RVSOC_SIM_V2_SIM_OBJECT_H
#define RVSOC_SIM_V2_SIM_OBJECT_H

#include <string>

namespace archlab::sim {

enum class LifecycleState {
    Constructed,
    Initialized,
    Ready,
    Running,
};

class SimObject {
public:
    explicit SimObject(std::string name);

    virtual ~SimObject() = default;

    SimObject(const SimObject&) = delete;
    SimObject& operator=(const SimObject&) = delete;
    SimObject(SimObject&&) = delete;
    SimObject& operator=(SimObject&&) = delete;

    [[nodiscard]] const std::string& name() const;
    [[nodiscard]] LifecycleState state() const;

    void initialize();
    void reset();
    void startup();

protected:
    virtual void on_initialize();
    virtual void on_reset();
    virtual void on_startup();
    virtual void on_shutdown();

private:
    std::string name_;
    LifecycleState state_ = LifecycleState::Constructed;
};

} // namespace archlab::sim

#endif // RVSOC_SIM_V2_SIM_OBJECT_H