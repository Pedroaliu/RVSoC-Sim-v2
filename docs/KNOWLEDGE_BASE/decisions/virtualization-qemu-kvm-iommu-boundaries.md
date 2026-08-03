# Decision — Virtualization, QEMU/KVM, and IOMMU Boundaries

> Status: Accepted architectural direction  
> Date: 2026-08-03

## 1. Decision summary

ArchLab will not implement “virtualization” as one monolithic subsystem. It will use explicit boundaries among:

1. guest architectural virtualization;
2. execution backends;
3. virtual-platform/device composition;
4. I/O virtualization and IOMMU;
5. timing models.

QEMU and KVM are adapters/execution mechanisms, not the internal semantic type system of RVSoC-Sim.

## 2. Execution backend contract

A backend must support a versioned contract equivalent to:

```text
load_arch_state(state)
run_until(stop_condition)
export_arch_state()
report_exit_or_stop_reason()
```

Optional capabilities include dirty-memory tracking, interrupt injection, breakpoint/ROI triggers and device-exit handling.

Rules:

- no backend pointer crosses into stable checkpoint or transaction formats;
- backend switches happen only at a quiescent or explicitly defined synchronization boundary;
- the run manifest records which backend executed each interval;
- KVM/TCG runtime is not reported as simulated guest cycles.

## 3. Platform composition contract

The first platform remains `archlab-rv64-virt-v0`.

QEMU may provide a functional frontend or reference device model. At an adapter boundary:

- QEMU address-space accesses convert to typed ArchLab requests;
- ArchLab completions convert back to QEMU-visible read/write/interrupt results;
- request identity and ordering are preserved explicitly;
- in-process callbacks are not serialized as the protocol;
- QEMU `MemoryRegion`, QOM and `DeviceState` pointers never become public ArchLab identity.

## 4. KVM contract

KVM is permitted as a fast functional backend for host-supported execution.

KVM does not supply:

- cache/NoC/DDR timing;
- deterministic cycle counts;
- device latency unless an explicit timing model is interposed;
- cross-architecture execution.

A VM exit is a backend control event. If an exit represents MMIO, the architectural access may enter the ArchLab platform adapter. Host time spent entering/exiting KVM is diagnostic host overhead, not guest hardware latency.

## 5. IOMMU request contract

A generic I/O translation request contains stable semantic fields:

```text
TransactionKey key
DeviceId device
optional ProcessId process
Address iova
AccessType access
Length length
TranslationMode mode
RequestAttributes attrs
```

It does not contain pointers to DDT/PDT entries, IOTLB lines, walker slots, queue elements, PCI functions or guest-kernel objects.

## 6. IOMMU translation result

A request resolves to exactly one semantic result family:

```text
Translated { spa, permissions, attributes }
Fault { cause, address, device/process identity }
PageRequest { page-request identity and required software action }
RetryNotAccepted { receiver did not accept ownership }
```

Transport backpressure is represented outside the translation result.

## 7. Guest-visible versus private IOMMU state

### Guest-visible or architecturally visible

- IOMMU MMIO registers;
- DDT/PDT/page tables in memory;
- command/fault/page-request queues;
- head/tail/status/interrupt state;
- specified performance-monitoring state when implemented.

### Private model state

- context caches;
- IOTLBs;
- page-walk caches;
- walker MSHRs/queue entries;
- arbitration state;
- device ATC tracking;
- internal retry tokens.

Checkpoint support must serialize all implemented state needed for deterministic continuation, while keeping private state out of public transactions.

## 8. Invalidation and fences

Invalidation commands and fences are first-class operations.

- An accepted invalidation owns a queue position and eventually completes or faults.
- A fence provides specification-defined ordering; it is not equivalent to draining the entire simulator EventQueue.
- Page-table writes alone do not implicitly invalidate every translation cache.
- Device ATC invalidation and IOMMU IOTLB invalidation are distinct.
- First-stage and second-stage invalidations retain their context scope.

## 9. ATS and PRI

ATS and PRI are deferred to a post-base milestone but influence interfaces now.

- ATS request/response is a separate protocol from DMA data transfer.
- A device ATC is private device state.
- ATC invalidation has explicit completion.
- PRI/page request is not an ordinary translation fault and may require guest/host software service.
- PASID/process identity must survive across request, fault/page request and completion.

## 10. Cross-project ownership

### RVSoC-Sim owns

- RISC-V CPU/privilege/MMU models;
- SoC platform/device/IOMMU models;
- transaction/timing backends;
- PCIe/NoC/DDR integration;
- QEMU/TCG/KVM adapters;
- ROI and fidelity switching.

### ArchLab-virt owns

- the self-built VMM learning path;
- VM/vCPU/run-loop/exit experiments;
- hardware virtualization and nested-translation labs;
- passthrough and IOMMU measurement experiments.

### Shared by specification or library only when stable

- architectural-state schema;
- address/fault/access enums;
- device/process identity types;
- checkpoint and trace formats;
- directed test vectors.

No forced repository merge is required merely because both projects study virtualization.

## 11. Staged implementation

### VIRT-0 — Functional base IOMMU

- one device context;
- Bare and one selected translated mode;
- permission checks and deterministic faults;
- MMIO configuration;
- directed unit tests.

### VIRT-1 — Two-stage and queues

- DDT and optional PDT;
- first/second-stage translation;
- command and fault queues;
- scoped invalidation and fences;
- checkpoint support.

### VIRT-2 — Device assignment integration

- PCIe requester identity;
- MSI translation/interrupt path;
- guest isolation tests;
- QEMU functional differential tests.

### VIRT-3 — ATS/PRI/SVA

- PASID/process contexts;
- device ATC;
- ATS and invalidation;
- page-request queue and software response;
- performance/timing resources.

### VIRT-4 — Timing and co-simulation

- walker/cache/NoC/DDR timing;
- queue/backpressure resources;
- QEMU/KVM fast-forward to ROI;
- detailed IOMMU/PCIe timing in ROI.

## 12. Validation gates

No stage is complete without:

- reset-state tests;
- valid and invalid translation tests;
- permissions and fault-code tests;
- invalidation/fence ordering tests;
- queue ownership/wrap/full tests where applicable;
- checkpoint/replay determinism;
- reference comparison against the selected QEMU/spec behavior;
- a run manifest declaring functional versus timing fidelity.
