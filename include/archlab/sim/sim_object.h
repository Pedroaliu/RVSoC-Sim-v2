#ifndef ARCHLAB_SIM_SIM_OBJECT_H
#define ARCHLAB_SIM_SIM_OBJECT_H

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

private:
    std::string name_;
    LifecycleState state_ = LifecycleState::Constructed;
};

} // namespace archlab::sim

#endif // ARCHLAB_SIM_SIM_OBJECT_H
