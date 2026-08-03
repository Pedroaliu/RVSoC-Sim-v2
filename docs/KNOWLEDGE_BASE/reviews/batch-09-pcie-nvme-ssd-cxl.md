# Detailed Review Batch 9 — PCIe, NVMe, SSD, and CXL

> Review level: L1, except one explicitly restricted source.  
> Date: 2026-08-03

This batch reviews seven canonical groups. L1 means that the actual table of contents, abstract/introduction and selected architectural sections were inspected. It does not mean cover-to-cover reading, and it does not replace the current PCI-SIG, NVM Express or CXL normative specifications.

## 1. Sources reviewed

### PCIE-001 — PCI Express Technology: Comprehensive Guide to Generations 1.x, 2.x, 3.0

The source provides the most complete PCIe structural map in the current library: topology and functions, configuration space, address routing, TLP request/completion/message types, posted and non-posted transactions, transaction ordering, credit flow control, DLLP Ack/Nak replay, error handling, LTSSM, power management and MSI.

**ArchLab use:** define the split between the PCIe function/configuration model, the transaction layer, data-link reliability, credit transport and the physical/link-state model.

**Do not copy literally:** this is a 2012 Gen3-era educational source. Later-generation capabilities and exact encodings must come from the selected normative specification.

### PCIE-002 — PCI Express 体系结构导读

This source is especially useful at the processor-system boundary. It covers host bridges and Root Complexes, PCI bridges and non-transparent bridges, posted/non-posted/split transfers, cache/prefetch interactions, the PCIe layers, link training and power, flow control, MSI/MSI-X, ordering, Linux use and virtualization.

**ArchLab use:** connect the CPU/SoC address map, Root Complex, configuration/enumeration, DMA and interrupt paths instead of modeling PCIe as an isolated packet link.

### SSD-001 — A Beginner's Guide to SSD Firmware

The inspected contents cover NAND and 3D NAND, NAND commands/interfaces, garbage collection, write amplification, over-provisioning, power-loss protection, restore points, FTL mapping, bad-block management, read/write paths, thermal throttling, health/event logging and exception handling.

**ArchLab use:** define internal SSD state and policies behind an NVMe-visible controller.

### SSD-002 — 深入浅出SSD（第二版）

The file is a 492-page scan. The actual rendered contents pages were inspected. The book spans SSD controllers, NAND reliability, FTL mapping/garbage collection/wear leveling/bad blocks/SLC cache, ECC and LDPC, PCIe topology/TLP/configuration/routing/link/power/SR-IOV, NVMe SQ/CQ/doorbells/namespaces/power/NVMe-oF/ZNS/CMB/HMB, and a broad SSD validation section including FTL, power-loss, endurance and compliance tests.

**ArchLab use:** the best end-to-end map in the library from PCIe frontend through NVMe controller to NAND backend and testing.

**Do not copy literally:** it is a secondary scanned source. Exact protocol fields and compliance rules must be sourced from official documents.

### PCIE-HOTPLUG-001 — Restricted vendor NVMe hotplug source

The document is visibly marked **AMD Confidential — NDA Required**. The public knowledge base therefore records only that it covers cross-layer platform/firmware/OS hotplug responsibilities, asynchronous add/remove and PCIe error containment/recovery categories.

**Repository rule:** do not quote, reproduce or commit detailed contents. Before implementation, replace this source with public PCIe hotplug/AER/DPC, ACPI and Linux documentation.

### STORAGE-001 — PCM-Based Durable Write Cache for Fast Disk I/O

The paper proposes a persistent PCM write cache in front of HDD storage, including hash-based mapping, random-write coalescing, destaging locality, crash recovery and wear-leveling. It is useful as a policy/modeling study, not as an NVMe protocol reference.

### CXL-001 — Performance Characterization of CXL Memory and Its Use Cases

The paper evaluates real CXL 1.1 Type-3 memory devices. It treats CXL memory as a CPU-less NUMA node and separates latency contributions from the PCIe path, CXL controller and CPU home agent. It also shows device/controller bandwidth saturation, workload-specific benefits and interactions among interleaving, placement and migration.

**ArchLab use:** calibrate a CXL memory backend and validate that a bottleneck can be in the device/controller rather than the raw PCIe link.

## 2. Batch-level architecture decisions

### 2.1 PCIe requests, link reliability and completion are different protocols

```text
ArchLab device/DMA request
        ↓ PCIe adapter
TLP request / message / completion
        ↓ data-link layer
sequence + LCRC + Ack/Nak + replay
        ↓ physical/link model
lane/link/LTSSM state
```

- A link-layer Ack confirms reliable TLP delivery across one link; it is **not** the completion of a non-posted PCIe transaction.
- Posted writes do not receive transaction completions.
- Non-posted requests consume a request identity/tag and later receive one or more completions.
- Link replay state and requester completion state are separate.

### 2.2 Credit backpressure is not protocol retry

PCIe credit classes distinguish posted, non-posted and completion traffic, with header/data resources. When a transmitter lacks credits, the packet remains owned and transport-blocked. This is not `RetryLater`, not request rejection and not a device-level retry.

### 2.3 Packetization is part of performance semantics

Max Payload Size, Max Read Request Size, alignment and completion boundaries determine how one upper-level transfer becomes multiple TLPs. The timing model must account for:

- per-packet header/link overhead;
- tag occupancy for non-posted operations;
- credit occupancy and update latency;
- completion fragmentation and reassembly;
- link width/rate and protocol encoding.

### 2.4 Stable identity versus private PCIe state

The PCIe adapter may map an ArchLab `TransactionKey` onto Requester ID + Tag and preserve byte-range information. Internal tag allocators, replay-buffer entries, flow-control counters, LTSSM states and switch-buffer pointers remain private model state.

### 2.5 NVMe host contract and SSD media backend are separate

```text
Guest-visible NVMe controller
    controller registers
    admin and I/O SQ/CQ
    doorbells
    namespaces
    interrupts
    command/status semantics
            ↓
Internal SSD backend
    FTL mapping
    NAND scheduling
    ECC/LDPC
    GC and wear leveling
    bad blocks and SLC cache
    power-loss recovery
    thermal/health state
```

Submission/completion queues are guest-visible memory. Queue cursors, controller work entries, FTL tables, NAND channel queues and GC tasks are internal state. A host command completion must obey the command's durability/ordering semantics; it cannot be emitted merely because an internal NAND request was scheduled.

### 2.6 Hotplug is a cross-layer asynchronous state machine

Presence, slot power, link state, configuration visibility, driver binding, outstanding I/O, error containment and OS/firmware ownership are separate state variables. Hot removal cannot be modeled as deleting a device object instantly.

### 2.7 CXL memory is not just “DRAM with a larger constant latency”

A useful CXL memory model preserves:

- host socket and home-agent placement;
- PCIe/CXL transport latency;
- device-controller latency and queueing;
- finite device bandwidth and early saturation;
- local/remote/CXL NUMA distance;
- page placement, interleaving and migration policy;
- workload-specific latency/bandwidth sensitivity.

## 3. Explicit source gaps

The library sources in this batch are sufficient for architectural decomposition and model planning, but **not** for final software-visible encodings. Before implementing the controller, the repository must pin versioned normative sources for:

1. PCI Express Base Specification and relevant ECNs/capabilities;
2. NVM Express Base and command-set specifications;
3. CXL specification for the selected version/device type;
4. public PCIe hotplug, AER/DPC, ACPI and Linux behavior.

## 4. Progress

- L1 source families before Batch 9: 37
- added in Batch 9: 7
- total L1 source families: **44**
- A canonical groups before Batch 9: 121 total, 13 covered, 108 not-yet-L1
- after Batch 9: **20 covered, 101 not-yet-L1**

## 5. Integrity

- normalized raw snapshot v5 SHA-256: `a67cd9899e5d7285f49072d7204d8943a0cf17dd091eb0c92808be175f442b97`
- A canonical coverage v3 SHA-256: `46f3ddf1cd881ec85c0f4be14c980eae9aeb675b162f69d535effbf2960fb321`
- Batch 9 inventory SHA-256: `76cd81417dc4b1b9ac6de54c58da37b59342bfee202e0b0f6ad56945971e5bf9`
