# Topic Map — PCIe, NVMe, SSD, and CXL

## PCIe

```text
Root Complex / Switch / Endpoint
    ├─ configuration and enumeration
    ├─ BAR / address routing
    ├─ TLP request, message and completion
    ├─ transaction ordering and attributes
    ├─ credit-based flow control
    ├─ data-link Ack/Nak and replay
    ├─ link/LTSSM/power
    └─ AER/DPC/hotplug
```

Primary ArchLab modules: PCIe RC, switch, endpoint adapter, DMA, MSI/MSI-X, IOMMU integration, timing link and trace/statistics.

## NVMe controller

```text
PCIe Function
    ↓ BAR/MMIO
NVMe registers and doorbells
    ↓
Admin SQ/CQ and I/O SQ/CQ
    ↓
Namespace and command execution
    ↓
SSD backend
```

Host-visible state and internal controller/media state must be modeled separately.

## SSD backend

```text
LBA request
    ↓ mapping lookup
FTL / placement
    ↓
channel / die / plane / block / page scheduling
    ↓
read-program-erase + ECC
    ↓
GC / wear / bad-block / recovery / thermal state
```

## CXL memory

```text
CPU load/store
    ↓ home agent
CXL.mem path over PCIe physical transport
    ↓ device controller / queue
media
```

CXL.io, CXL.cache and CXL.mem are not interchangeable. The first ArchLab CXL memory backend should target a named device type and version rather than a generic “CXL” flag.

## Validation ladder

1. register/configuration directed tests;
2. request/completion/tag/credit invariants;
3. link replay and error injection;
4. NVMe queue/doorbell/interrupt tests;
5. FTL/GC/power-loss/media fault tests;
6. Linux driver and hotplug tests;
7. throughput/latency calibration;
8. CXL NUMA placement and saturation experiments.
