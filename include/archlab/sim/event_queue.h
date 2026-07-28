#ifndef ARCHLAB_SIM_EVENT_QUEUE_H
#define ARCHLAB_SIM_EVENT_QUEUE_H

#include "archlab/sim/time.h"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <iosfwd>
#include <optional>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

namespace archlab::sim {

class EventHandle {
public:
    EventHandle() = default;

    [[nodiscard]] bool valid() const noexcept {
        return id_ != 0;
    }

    [[nodiscard]] std::uint64_t value() const noexcept {
        return id_;
    }

    auto operator<=>(const EventHandle&) const = default;

private:
    explicit EventHandle(std::uint64_t id) : id_(id) {
    }

    std::uint64_t id_ = 0;

    friend class EventQueue;
};

class EventQueue {
public:
    using Callback = std::function<void()>;

    EventQueue() = default;

    [[nodiscard]] const TimePoint& current_time() const noexcept;
    [[nodiscard]] Tick now() const noexcept;
    [[nodiscard]] std::size_t pending_events() const noexcept;
    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] bool stop_requested() const noexcept;
    [[nodiscard]] std::optional<TimePoint> next_event_time() const;

    EventHandle schedule_at(TimePoint when, std::string name, Callback callback);
    EventHandle schedule_abs(Tick when, std::string name, Callback callback);
    EventHandle schedule_after(Tick delay, std::string name, Callback callback);

    bool cancel(EventHandle handle);

    void run();
    void run_until(TimePoint until);
    void run_until(Tick until);

    void request_stop() noexcept;
    void clear_stop() noexcept;

    void set_trace_stream(std::ostream* stream) noexcept;

private:
    struct Event {
        TimeStamp stamp{};
        std::uint64_t id = 0;
        std::string name;
        Callback callback;
    };

    struct EventCompare {
        bool operator()(const Event& lhs, const Event& rhs) const noexcept;
    };

    void process_next_event();
    void discard_cancelled_front();

    TimePoint current_{};
    Sequence next_sequence_ = 0;
    std::uint64_t next_id_ = 1;
    bool stop_requested_ = false;
    std::ostream* trace_stream_ = nullptr;

    std::priority_queue<Event, std::vector<Event>, EventCompare> events_;
    std::unordered_set<std::uint64_t> live_ids_;
    std::unordered_set<std::uint64_t> cancelled_ids_;
};

} // namespace archlab::sim

#endif // ARCHLAB_SIM_EVENT_QUEUE_H
