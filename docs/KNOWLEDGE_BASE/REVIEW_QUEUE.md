# Knowledge Review Queue

> Last updated: 2026-08-03

审阅顺序服从 ArchLab 当前路线，不以文件上传时间排序。每项先达到 L1，再决定是否进入 L2/L3。每批完成后立即写入仓库，不依赖聊天记忆积累多批结果。

## Batch 1 — Simulation foundation

Status: **L1 complete**

- Fundamentals of Full-Platform Simulation
- Discrete-Event System Simulation
- System Design with SystemC
- Transaction Level Modeling with SystemC
- gem5 + SystemC integration
- Full-System Simulation with Simics
- SimBricks
- Ramulator 2.0

Record: `reviews/batch-01-simulation-foundation.md`

## Batch 2 — CPU / ISA / OOO and SoC boundaries

Status: **L1 complete**

1. Modern Processor Design: Fundamentals of Superscalar Processors
2. Computer Architecture: A Quantitative Approach, 6th ed.
3. Processor Microarchitecture: An Implementation Perspective
4. Modern System-on-Chip Design on Arm (`ModernSoC_textbook.pdf`)

Records:

- `reviews/batch-02-cpu-isa-ooo.md`
- `topics/cpu.md`
- `inventory/batch-02-cpu-isa-ooo.csv`

## Batch 3 — Memory / Cache / Coherence / NoC

Status: **L1 complete**

1. Memory Systems: Cache, DRAM, Disk
2. The Memory System: You Can’t Avoid It, You Can’t Ignore It, You Can’t Fake It
3. A Primer on Memory Consistency and Cache Coherence, 2nd ed.
4. On-Chip Networks, 2nd ed.
5. GARNET paper

Records:

- `reviews/batch-03-memory-coherence-noc.md`
- `topics/memory.md`
- `topics/noc.md`
- `decisions/memory-coherence-noc-boundaries.md`
- `inventory/batch-03-memory-coherence-noc.csv`

## Batch 4 — Performance / Datacenter / Methodology

Status: **L1 complete**

1. The Art of Computer Systems Performance Analysis
2. Performance Modeling and Design of Computer Systems
3. The Datacenter as a Computer, Second and Third Editions
4. Computer Architecture Performance Evaluation Methods

Records:

- `reviews/batch-04-performance-datacenter-methodology.md`
- `topics/performance.md`
- `decisions/performance-evaluation-methodology.md`
- `inventory/batch-04-performance-methodology.csv`

## Batch 5 — Linux MM / Virtual Memory foundation

Status: **L1 complete**

1. The Linux Memory Manager — Lorenzo Stoakes
2. Operating Systems: Three Easy Pieces / OSTEP family
3. Architectural and Operating System Support for Virtual Memory
4. Local and Remote Memory: Memory in a Linux/NUMA System

Completed extraction:

- VMA validity versus physical-page allocation versus PTE presence;
- `mm_struct`/VMA Linux implementation role;
- abstraction, mechanism and policy separation;
- TLB, page table, page walker, permission and fault boundaries;
- page tables as guest memory and TLB/walker queues as model state;
- demand paging, CoW, anonymous/file-backed mappings and fault retry;
- Linux guest policy versus ArchLab hardware responsibility;
- Linux-aware decoder boundaries;
- NUMA execution, memory-home and device-locality separation;
- functional versus timed translation hierarchy;
- source gap for the RISC-V-specific boot platform.

Records:

- `reviews/batch-05-linux-mm-virtual-memory.md`
- `topics/linux.md`
- `decisions/linux-vm-full-system-boundaries.md`
- `inventory/batch-05-linux-mm-vm.csv`

## Batch 5B — RISC-V Linux boot contracts

Status: **next / source discovery required**

This batch exists because the Linux/VM sources do not define the exact RISC-V platform contract. Use authoritative, versioned sources wherever possible.

Required source families:

1. RISC-V privileged architecture specification for the selected implementation baseline;
2. Sv39 PTE, `satp`, ASID, permission, A/D-bit and `SFENCE.VMA` rules;
3. trap, interrupt, delegation and return behavior;
4. timer and selected interrupt-controller specification (the project must name the exact platform model);
5. device-tree specification and relevant RISC-V/platform bindings;
6. OpenSBI firmware interfaces, domain/platform handoff and console/timer services;
7. Linux RISC-V boot requirements and expected register/DTB state;
8. selected UART and minimal MMIO-device specifications;
9. Spike/NEMU/QEMU reference behavior and differential-test limits.

Expected extraction:

- exact first-machine memory map;
- reset PC and boot ROM behavior;
- privilege transition and CSR contract;
- precise Sv39 translation and fault outcomes;
- timer/interrupt event path;
- DTB contents and boot handoff;
- minimum UART behavior;
- OpenSBI-to-Linux sequence;
- directed and differential validation matrix;
- authoritative version identifiers stored in the repository.

Batch 5B is complete only when the first Linux-machine design can cite a source for every software-visible platform behavior.

## Batch 6 — R-class manual triage

Status: **43 raw records queued after Batch 5B**

For each R item:

1. identify bibliographic title from the first pages;
2. inspect table of contents or abstract;
3. assign canonical work and domain;
4. decide S/A/B/C;
5. record whether the filename was opaque, truncated or misleading;
6. immediately promote any newly discovered S source into its own review batch.

The R batch comes before broad A-class review so an important source is not lost merely because its filename was poor.

## A-class milestone review

Status: **145 raw records in the original coarse pool; remaining canonical count unknown**

Some A-class records have already been absorbed into canonical batches. The complete remaining count will be calculated only after the normalized inventory and duplicate relationships are complete.

Planned groups:

1. Virtualization / QEMU / KVM / IOMMU
2. PCIe / NVMe / SSD / CXL
3. RAS / ECC / Chipkill / fault injection / recovery
4. Parallel / NUMA / HPC / synchronization
5. Compiler / assembly / JIT / binary translation
6. GPU / NPU / DPU / SIMT / collective communication
7. Power / thermal / energy
8. Security / confidential computing
9. Firmware / boot / UEFI / OpenSBI
10. Programming and software-engineering references that directly affect simulator implementation

## B-class references

Status: **398 raw records retained**

B-class files remain searchable and classified. They are read on demand when:

- an ArchLab milestone needs the subject;
- an A/S source cites a useful implementation or validation method;
- a new design question exposes a gap in the primary source set.

They are not scheduled for indiscriminate cover-to-cover reading.
