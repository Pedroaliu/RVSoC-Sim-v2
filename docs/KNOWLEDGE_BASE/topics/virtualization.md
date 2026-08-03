# Topic Map — Virtualization / QEMU / KVM / IOMMU

## 1. Why this topic is split

“Virtualization” spans four different contracts:

```text
Architectural virtualization
Execution acceleration
Virtual-platform/device composition
I/O virtualization
```

Treating them as one object would mix software-visible semantics, host execution mechanisms, device behavior and timing fidelity.

## 2. Architectural virtualization

Questions:

- Which privilege levels and virtual control state exist?
- Which operations execute directly and which trap/exit?
- How are guest exceptions and interrupts delivered?
- How is GVA translated through GPA to HPA?
- Which state is guest-visible, hypervisor-visible or implementation-private?

ArchLab rule:

- architectural results and faults are stable contracts;
- implementation-specific VMCS/VMCB/KVM structures remain backend-private;
- functional and timing implementations must agree on architectural state.

## 3. Execution backends

```text
Interpreter / self-developed functional core
QEMU TCG
KVM
Trace frontend
Timing core
RTL/Verilator
```

KVM/TCG accelerate execution but do not define timing truth. A backend switch requires an explicit architectural-state/checkpoint contract and an experiment manifest recording which backend executed each interval.

## 4. QEMU platform composition

Useful QEMU concepts:

- Machine/board selects a complete software-visible platform;
- QOM provides type/object/property/realize composition;
- AddressSpace defines a viewpoint;
- MemoryRegion composes RAM, ROM, MMIO, containers and aliases;
- MemoryListener propagates map changes to consumers such as KVM;
- device models implement register, interrupt and DMA behavior.

ArchLab mapping:

```text
QEMU Machine          → Platform configuration / builder
QEMU Device/QOM       → SimObject/device instance concept
AddressSpace          → address-decode viewpoint
MemoryRegion          → mapped target/region adapter
MemoryListener        → configuration/change notification
QEMU callback         → adapter edge, not internal transaction identity
```

ArchLab does not recreate QOM. Its typed C++ interfaces and configuration own the internal model.

## 5. KVM split

KVM’s essential architecture is:

```text
QEMU/userspace
    creates VM and vCPUs
    maps guest memory
    constructs devices
    handles exits not consumed in kernel

Linux KVM
    executes vCPUs with hardware assistance
    owns architecture-specific virtualization state
    reports exit reason/state through the KVM ABI
```

ArchLab use:

- KVM can run non-ROI software quickly;
- the ROI boundary switches through serialized architectural/platform state;
- KVM memory slots are host execution mappings, not DRAM/cache timing models;
- VM exits become backend events, not modeled cycle latency by default.

## 6. IOMMU model

### 6.1 Generic inputs

- device identity;
- optional process/PASID identity;
- IOVA and address type;
- access type and length;
- transaction attributes;
- translation-context selection.

### 6.2 Generic outputs

- translated system physical address and attributes;
- permission/configuration/page fault;
- page request requiring software service;
- explicit completion for invalidation/fence/ATS operations.

### 6.3 State layering

```text
Guest-visible memory
    DDT/PDT/page tables/CQ/FQ/PQ

Architectural IOMMU registers
    base pointers/head/tail/status/control

Private timing state
    context cache/IOTLB/walker/queue entry/ATC tracking
```

Private entries never replace `TransactionKey`.

## 7. Device assignment and shared virtual addressing

### Passthrough

- guest programs the device;
- IOMMU second-stage translation enforces guest isolation;
- interrupt/MSI routing must be virtualized;
- reset, ownership transfer and DMA quiescence are part of assignment.

### SVA

- device carries process identity/PASID;
- device and CPU may share page tables;
- ATS lets a device cache translations in its ATC;
- invalidation keeps ATC coherent with page-table changes;
- PRI/page request lets software resolve missing mappings.

## 8. Retry and completion taxonomy

```text
Transport backpressure
    message cannot move yet

Queue full before acceptance
    command/request not owned

Accepted command
    IOMMU owes completion or fault record

Translation fault
    request completed with architectural failure

Page request
    translation is deferred pending software service

ATS/ATC invalidation
    separate protocol and completion domain
```

These conditions cannot be represented by one boolean return.

## 9. Cross-project boundary

### RVSoC-Sim

- full RISC-V machine;
- functional/timing/cycle backends;
- device and IOMMU models;
- cache/NoC/DDR/PCIe timing;
- stats, trace, ROI and fidelity switching.

### ArchLab-virt

- educational VMM implementation;
- virtualization control flow;
- VM/vCPU/memory-slot/exit experiments;
- hardware-extension and passthrough labs.

### Shared

- architectural state schema;
- fault/exit taxonomy;
- transaction and device identity types;
- source-backed machine specification;
- differential tests and traces;
- QEMU/KVM adapters.

## 10. Source hierarchy

For implementation details use this order:

1. selected architecture/IOMMU specification;
2. current Linux KVM and QEMU source/API documentation;
3. project tests and observed behavior;
4. reviewed books for explanation and historical call paths.

The 2013 KVM book and QEMU-2.8.1/Linux-4.4.161 source-analysis book are explanatory references, not current API contracts.
