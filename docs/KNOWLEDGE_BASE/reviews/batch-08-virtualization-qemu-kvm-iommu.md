# Batch 8 — Virtualization, QEMU, KVM, and IOMMU

> Review level: **L1**  
> Date: 2026-08-03

L1 here means that the actual table of contents, abstract/introduction, and selected architecture or implementation sections were inspected. It does not mean every book was read cover-to-cover. Version-specific QEMU/KVM code paths must be rechecked against the versions selected for implementation.

## 1. VIRT-001 — RISC-V IOMMU Architecture Specification

- **Raw record:** K0002, `riscv-iommu (1).pdf`
- **Version inspected:** v1.0.1, ratified, 2024-09-11
- **Drive ID:** `1FfUSbHMUo1ssvodKCmN0GapC6yofUxOr`
- **Priority after review:** S

### Verified scope

The specification defines a system IOMMU for DMA-capable devices. The inspected material covers:

- non-virtualized, hypervisor and guest usage models;
- per-device identity and Device Directory Table (DDT);
- optional per-process identity and Process Directory Table (PDT);
- first-stage and second-stage translation using CPU-compatible page-table formats;
- device/process contexts, permissions and A/D updates;
- MSI translation;
- PCIe ATS translation requests and page requests;
- translation/data-structure caching and invalidation;
- command, fault/event and page-request in-memory queues;
- MMIO register interface, reset and interrupts;
- software initialization, invalidation and emulation guidelines;
- QoS identifiers and ATC capacity monitoring.

### ArchLab value

This becomes the authoritative contract for the future RISC-V IOMMU model. It is also the cleanest source in the current library for separating:

```text
device/process identity
    ↓
translation context
    ↓
first/second-stage walk
    ↓
DMA memory transaction
    ↓
completion or fault/event/page request
```

### Project implications

- The public DMA request carries stable device/process identity, not a pointer to a DDT/PDT/cache entry.
- DDT/PDT/page tables and guest-visible queues live in simulated memory.
- IOTLB/context caches, walker entries and queue cursors are subsystem-private model state.
- First-stage, second-stage and Bare modes share one translation-result contract.
- Invalidations and fences are ordered commands, not ad-hoc cache flush callbacks.
- ATS/ATC and PRI/page request are explicit protocols with their own request, completion and invalidation paths.

### Do not copy literally

Do not expose the specification’s internal table or queue indices as ArchLab `TransactionKey`. Do not implement every optional extension in the first IOMMU milestone.

## 2. VIRT-002 — How to Develop Embedded Software Using the QEMU Machine Emulator

- **Raw record:** K0006
- **Author:** Artem Kotovsky / Apriorit
- **Drive ID:** `1bc7z8HkPEJ0E2PbCDCXs147bm5MCgriS`
- **Priority after review:** A

### Verified scope

The guide builds a mock PCI device in QEMU and a Windows WDF driver around a defined device/driver contract. It covers:

- benefits and limits of developing software before physical hardware exists;
- PCI I/O/MMIO address space;
- INTx and MSI;
- bus mastering and DMA;
- device I/O layout and request protocol;
- QEMU device initialization, MMIO callbacks, interrupts and DMA;
- driver resource initialization and request flow;
- unit, automated and verifier-based testing.

### ArchLab value

It is a practical device-contract reference. The useful lesson is not the Windows API; it is the workflow:

```text
write an explicit software-visible device specification
    ↓
implement a functional virtual device
    ↓
run the real driver against it
    ↓
use tests to validate the specification before hardware exists
```

This directly supports future PCIe/NVMe/IOMMU device-model development.

### Limits

The guide explicitly notes that a virtual mock device is slower and cannot validate all synchronization, boundary or physical timing failures. Therefore a QEMU functional device is a correctness oracle, not a latency/bandwidth model.

## 3. VIRT-003 — KVM虚拟化技术：实战与原理解析

- **Raw record:** K0007
- **Authors:** 任永杰、单海涛
- **Edition inspected:** 2013
- **Drive ID:** `1R9g6TTi1ghh16OsVJUUwLs1k-exxZT9z`
- **Priority after review:** A

### Verified scope

The book covers KVM architecture and modules, the QEMU device model, Intel hardware virtualization, VM construction, CPU/memory/storage/network configuration, virtio, VT-d device assignment, hotplug, live migration, nested virtualization, KSM, security, management tools and performance testing.

### ArchLab value

It gives a broad system map and makes the QEMU/KVM split clear:

- KVM is a Linux-kernel virtualization facility using hardware assistance;
- QEMU provides userspace machine construction and device models;
- virtio and direct assignment are distinct device paths;
- CPU, memory, device and migration state must be considered separately.

### Limits

This is a 2013 source. It is valuable for concepts and workflow but not authoritative for current ioctls, QEMU object APIs, interrupt virtualization, confidential computing or modern IOMMU behavior.

## 4. VIRT-004 — Hardware Support for Efficient Virtualization

- **Raw record:** K0011, `hardwareVirt.pdf`
- **Resolved title:** *Hardware Support for Efficient Virtualization*
- **Drive ID:** `10wgr1Irr6LuwEvmHvfSgzun88Dfy2D-q`
- **Priority after review:** A

### Verified scope

This survey reviews:

- the classical Popek–Goldberg goals of equivalent execution, performance and resource control;
- trap-and-emulate and the sensitive/privileged instruction condition;
- IBM VM/370, interpretive execution and historical hardware assists;
- software techniques for non-classically virtualizable x86;
- Intel/AMD CPU virtualization support;
- shadow page tables versus hardware two-dimensional translation;
- I/O and MMU virtualization.

### ArchLab value

It provides the conceptual vocabulary needed to keep three questions separate:

1. Does the guest observe architecturally correct behavior?
2. Which instructions execute directly versus exit/emulate?
3. Who owns and controls each resource?

### Limits

It is an older survey and must not be used as a current hardware specification. Its IBM and early x86 material is retained as design history and conceptual comparison.

## 5. VIRT-005 — AMD I/O Virtualization Technology (IOMMU) Specification

- **Raw record:** K0014, `AMD_IOMMU.pdf`
- **Version inspected:** Publication 48882-PUB, Revision 3.10, February 2025
- **Drive ID:** `13QO1_GW7ajD_O4MC00M4HGG7FzG4nABp`
- **Priority after review:** A

### Verified scope

The current AMD specification covers:

- host and guest device access;
- IOMMU virtualization;
- two-level guest/host translation;
- processor-page-table compatibility;
- ATS for guest virtual addresses and PRI-compatible page requests;
- interrupt virtualization;
- A/D-bit support;
- guest I/O protection;
- hardware-accelerated vIOMMU;
- SEV-SNP interaction;
- device tables, command/event mechanisms, invalidation and fault reporting.

### ArchLab value

AMD IOMMU is a comparative architecture source. Together with RISC-V IOMMU it shows which concepts are general:

- device identity and translation context;
- nested translation;
- device translation caches and invalidation;
- page requests;
- interrupt/MSI remapping;
- guest-visible command/event interfaces;
- isolation and fault reporting.

### Do not copy literally

ArchLab’s RISC-V model follows the RISC-V specification. AMD names, encodings and tables remain adapter/reference material, not the internal generic IOMMU API.

## 6. VIRT-006 — QEMU/KVM源码解析与应用

- **Raw record:** K0016
- **Author:** 李强
- **Publication:** 2021
- **Source baselines used by the book:** QEMU 2.8.1, Linux 4.4.161, SeaBIOS around rel-1.11.2
- **Drive ID:** `1f3WOBuMeIVVpxxvaKlJHqWrXrzY3WBRL`
- **Priority after review:** A

### Verified scope

The inspected material covers:

- QEMU command-line option construction;
- QOM types, classes, objects, properties and realize;
- Machine selection and board composition;
- CPU objects and KVM-backed vCPU threads;
- AddressSpace and MemoryRegion hierarchy;
- RAM, ROM, MMIO, container and alias regions;
- MemoryListener and KVM memory slots;
- `/dev/kvm`, VM/vCPU creation and the `KVM_RUN` loop;
- VM exits returned through shared `kvm_run` state;
- userspace handling of I/O/MMIO exits;
- interrupt-controller split and virtio/device construction.

### ArchLab value

This is the strongest implementation-structure source in the batch. It demonstrates four separations we should preserve:

```text
Machine / board composition
Device object and software-visible register behavior
Address-space decode and backing storage
Execution backend and exit handling
```

QEMU’s AddressSpace/MemoryRegion graph is particularly useful as a reference for memory-map composition. `MemoryListener` illustrates how a change in one authoritative map can notify KVM and other consumers.

### Limits

The exact functions and object hierarchy are tied to QEMU 2.8.1 and Linux 4.4.161. Modern implementation work must re-read current upstream source. ArchLab should borrow the separation of concerns, not reproduce QOM or old call graphs.

## 7. Batch-level conclusions

### 7.1 Virtualization is not one module

```text
Guest architectural virtualization
    privilege / traps / virtual CSRs / second-stage translation

Execution backend
    interpreter / TCG / KVM / trace

Platform virtualization
    machine / address map / interrupt / device models

I/O virtualization
    DMA / IOMMU / ATS / PRI / assignment / vIOMMU
```

They share contracts but need separate implementations and fidelity declarations.

### 7.2 QEMU/KVM split

```text
QEMU userspace
    machine construction
    address-space composition
    device models
    firmware/image loading
    userspace exit handling

KVM kernel
    VM/vCPU execution
    hardware virtualization state
    in-kernel acceleration where supported
    exits to userspace for unhandled operations
```

KVM is a fast execution backend. It is not a cycle model, and VM-exit duration must not be interpreted as modeled guest device latency.

### 7.3 ArchLab project boundary

- **RVSoC-Sim** owns the RISC-V architectural machine, timing backends, devices, IOMMU, SoC interconnect and stats.
- **ArchLab-virt** owns the educational/experimental VMM/KVM-style implementation path.
- Shared artifacts are specifications, architectural-state serialization, transaction/fault types, differential tests and adapters—not a forced monolithic codebase.
- QEMU and KVM connect through explicit adapters; neither QOM objects nor kernel pointers become ArchLab public identities.

### 7.4 IOMMU boundary

A generic request must preserve at least:

```text
TransactionKey
DeviceId
optional ProcessId/PASID
IOVA/address type
read/write/execute/atomic intent
length and attributes
translation mode/context selector
```

The result is either a translated SPA plus permissions/attributes or an explicit fault/page-request outcome. DDT/PDT entries, IOTLB slots and walker structures remain private.

### 7.5 Ordering

- A command accepted into a guest-visible command queue is owned by the IOMMU until completion/fault.
- An invalidation fence orders earlier invalidations and observed translations; it is not a generic event-queue flush.
- ATS translation completion, ATC invalidation completion and PRI page response are distinct completions.
- Queue full/backpressure, malformed command, translation fault and page-request deferral must not collapse into one `RetryLater` result.

## 8. Validation matrix

The future implementation should include directed tests for:

1. Bare, first-stage-only, second-stage-only and two-stage translation;
2. missing/invalid device and process contexts;
3. read/write/execute permission faults and A/D updates;
4. context/page-table cache invalidation plus fence ordering;
5. command, fault and page-request queue wrap/full/overflow behavior;
6. MSI translation;
7. ATS hit/miss, ATC invalidation and PRI request/response;
8. device assignment isolation across guests;
9. QEMU functional-device differential tests;
10. KVM/TCG/ArchLab architectural-state agreement at switch boundaries.

## 9. Priority decision

- RISC-V IOMMU specification is promoted to **S** as the authoritative future implementation contract.
- The other five works remain **A**: essential implementation/comparison material, but version- or platform-specific.
