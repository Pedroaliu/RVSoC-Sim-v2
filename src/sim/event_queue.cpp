#include "archlab/sim/event_queue.h"

#include <limits>
#include <ostream>
#include <stdexcept>
#include <utility>

namespace archlab::sim {

const TimePoint& EventQueue::current_time() const noexcept {
    return current_;
}

Tick EventQueue::now() const noexcept {
    return current_.tick;
}

std::size_t EventQueue::pending_events() const noexcept {
    return live_ids_.size();
}

bool EventQueue::empty() const noexcept {
    return live_ids_.empty();
}

bool EventQueue::stop_requested() const noexcept {
    return stop_requested_;
}

std::optional<TimePoint> EventQueue::next_event_time() const {
    auto copy = events_;
    while (!copy.empty()) {
        const Event& event = copy.top();
        if (!cancelled_ids_.contains(event.id)) {
            return event.stamp.time;
        }
        copy.pop();
    }
    return std::nullopt;
}

EventHandle EventQueue::schedule_at(TimePoint when, std::string name, Callback callback) {
    if (when < current_) {
        throw std::runtime_error("cannot schedule event in the simulated past: " + name);
    }
    if (!callback) {
        throw std::invalid_argument("event callback must not be empty: " + name);
    }
    if (next_id_ == 0) {
        throw std::overflow_error("event identifier space exhausted");
    }
    if (next_sequence_ == std::numeric_limits<Sequence>::max()) {
        throw std::overflow_error("event sequence space exhausted");
    }

    const std::uint64_t id = next_id_++;
    events_.push(Event{
        .stamp = TimeStamp{.time = when, .sequence = next_sequence_++},
        .id = id,
        .name = std::move(name),
        .callback = std::move(callback),
    });
    live_ids_.insert(id);
    return EventHandle{id};
}

EventHandle EventQueue::schedule_abs(Tick when, std::string name, Callback callback) {
    return schedule_at(at_tick(when), std::move(name), std::move(callback));
}

EventHandle EventQueue::schedule_after(Tick delay, std::string name, Callback callback) {
    if (delay == 0) {
        return schedule_at(next_delta(current_), std::move(name), std::move(callback));
    }
    if (delay > std::numeric_limits<Tick>::max() - current_.tick) {
        throw std::overflow_error("relative event time exceeds Tick range");
    }
    return schedule_abs(current_.tick + delay, std::move(name), std::move(callback));
}

bool EventQueue::cancel(EventHandle handle) {
    if (!handle.valid() || !live_ids_.contains(handle.id_)) {
        return false;
    }

    live_ids_.erase(handle.id_);
    cancelled_ids_.insert(handle.id_);
    return true;
}

void EventQueue::run() {
    while (!stop_requested_) {
        discard_cancelled_front();
        if (events_.empty()) {
            break;
        }
        process_next_event();
    }
}

void EventQueue::run_until(TimePoint until) {
    if (until < current_) {
        throw std::runtime_error("cannot run simulation backward");
    }

    while (!stop_requested_) {
        discard_cancelled_front();
        if (events_.empty() || events_.top().stamp.time > until) {
            break;
        }
        process_next_event();
    }

    if (!stop_requested_) {
        current_ = until;
    }
}

void EventQueue::run_until(Tick until) {
    run_until(end_of_tick(until));
}

void EventQueue::request_stop() noexcept {
    stop_requested_ = true;
}

void EventQueue::clear_stop() noexcept {
    stop_requested_ = false;
}

void EventQueue::set_trace_stream(std::ostream* stream) noexcept {
    trace_stream_ = stream;
}

bool EventQueue::EventCompare::operator()(const Event& lhs, const Event& rhs) const noexcept {
    return lhs.stamp > rhs.stamp;
}

void EventQueue::process_next_event() {
    Event event = events_.top();
    events_.pop();

    live_ids_.erase(event.id);
    current_ = event.stamp.time;

    if (trace_stream_ != nullptr) {
        *trace_stream_ << "[event] tick=" << event.stamp.time.tick
                       << " phase=" << event.stamp.time.phase
                       << " delta=" << event.stamp.time.delta
                       << " seq=" << event.stamp.sequence << " id=" << event.id
                       << " name=" << event.name << '\n';
    }

    event.callback();
}

void EventQueue::discard_cancelled_front() {
    while (!events_.empty() && cancelled_ids_.contains(events_.top().id)) {
        cancelled_ids_.erase(events_.top().id);
        events_.pop();
    }
}

} // namespace archlab::sim
