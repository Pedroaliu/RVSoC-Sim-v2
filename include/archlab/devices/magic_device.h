#ifndef ARCHLAB_DEVICES_MAGIC_DEVICE_H
#define ARCHLAB_DEVICES_MAGIC_DEVICE_H

#include "archlab/platform/mmio_device.h"
#include "archlab/sim/event_queue.h"

#include <iosfwd>
#include <string>
#include <string_view>

namespace archlab::stats {
class StatsManager;
}

namespace archlab::devices {

enum class MagicCommand : Data {
    Nop = 0,
    ResetStats = 1,
    DumpStats = 2,
    StopSimulation = 3,
    BeginRoi = 4,
    EndRoi = 5,
};

constexpr Data encode(MagicCommand command) noexcept {
    return static_cast<Data>(command);
}

class MagicDevice final : public platform::MmioDevice {
public:
    MagicDevice(sim::EventQueue& event_queue, std::string name,
                stats::StatsManager* stats = nullptr, std::ostream* output = nullptr);

    [[nodiscard]] std::string_view name() const noexcept override;
    [[nodiscard]] Data read(Addr offset, platform::AccessSize size) override;
    void write(Addr offset, platform::AccessSize size, Data value) override;

private:
    void validate_access(Addr offset, platform::AccessSize size) const;
    void execute(MagicCommand command);
    [[nodiscard]] static std::string_view command_name(MagicCommand command) noexcept;

    sim::EventQueue& event_queue_;
    std::string name_;
    stats::StatsManager* stats_;
    std::ostream* output_;
    Data last_command_ = encode(MagicCommand::Nop);
};

} // namespace archlab::devices

#endif // ARCHLAB_DEVICES_MAGIC_DEVICE_H
