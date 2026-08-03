# Knowledge Base Status

> Last updated: 2026-08-03

This file is the cross-conversation handoff point for ArchLab source review. It distinguishes asset inventory, filename classification, actual content review, and implementation extraction.

## 1. Full asset inventory

L0 filename-level screening is complete for **623 PDF records**:

- Books: 520;
- Paper folder: 103;
- original coarse priorities: S 36, A 145, B 398, R 43, C 1;
- 62 raw records were placed into about 31 preliminary duplicate groups.

The duplicate result is still primarily title-based. Raw records are preserved until edition, size, metadata and content relationships are confirmed.

## 2. L1 actual-content review

**31 canonical works/source families** have reached L1.

### Batch 1 — Simulation foundation (8)

- Fundamentals of Full-Platform Simulation;
- Discrete-Event System Simulation;
- System Design with SystemC;
- Transaction Level Modeling with SystemC;
- gem5 + SystemC integration;
- Full-System Simulation with Simics;
- SimBricks;
- Ramulator 2.0.

Record: `reviews/batch-01-simulation-foundation.md`

### Batch 2 — CPU / ISA / OOO / SoC boundaries (4)

- Modern Processor Design;
- Computer Architecture: A Quantitative Approach, Sixth Edition;
- Processor Microarchitecture;
- Modern System-on-Chip Design on Arm.

Records: `reviews/batch-02-cpu-isa-ooo.md`, `topics/cpu.md`.

### Batch 3 — Memory / Coherence / NoC (5)

- Memory Systems: Cache, DRAM, Disk;
- The Memory System;
- A Primer on Memory Consistency and Cache Coherence, Second Edition;
- On-Chip Networks, Second Edition;
- GARNET.

Records: `reviews/batch-03-memory-coherence-noc.md`, `topics/memory.md`, `topics/noc.md`, `decisions/memory-coherence-noc-boundaries.md`.

### Batch 4 — Performance / Datacenter / Methodology (4)

- The Art of Computer Systems Performance Analysis;
- Performance Modeling and Design of Computer Systems;
- The Datacenter as a Computer;
- Computer Architecture Performance Evaluation Methods.

Records: `reviews/batch-04-performance-datacenter-methodology.md`, `topics/performance.md`, `decisions/performance-evaluation-methodology.md`.

### Batch 5 — Linux MM / Virtual Memory foundation (4)

- The Linux Memory Manager;
- Operating Systems: Three Easy Pieces;
- Architectural and Operating System Support for Virtual Memory;
- Local and Remote Memory in a Linux/NUMA System.

Records: `reviews/batch-05-linux-mm-virtual-memory.md`, `topics/linux.md`, `decisions/linux-vm-full-system-boundaries.md`.

### Batch 5B — RISC-V Linux boot contracts (6)

- RISC-V Privileged Architecture `v20260120`;
- RISC-V SBI v3.0;
- Linux RISC-V Kernel Boot Requirements;
- OpenSBI platform requirements and firmware model;
- Devicetree Specification;
- upstream QEMU RISC-V `virt` platform documentation/source.

Records:

- `reviews/batch-05b-riscv-linux-boot-contracts.md`;
- `topics/riscv-linux-platform.md`;
- `decisions/riscv-linux-machine-v0.md`;
- `inventory/batch-05b-riscv-linux-boot.csv`.

## 3. Accepted cross-source conclusions

### Simulator framework

- stable architectural-state and semantic transaction contracts;
- fidelity and transport behind adapters;
- explicit, serialization-ready completion;
- deterministic ordering, trace, validation and checkpoint capability;
- single-process protocol correctness before PDES/distributed execution.

### CPU / OOO

- architectural state is distinct from microarchitectural state;
- functional, timing in-order and detailed OOO models have explicit boundaries;
- execution completion, memory completion and commit are different events;
- ROB/LSQ/MSHR indices are private state, not public transaction identity.

### Memory / Coherence / NoC

- ISA memory model, core ordering, coherence, network transport and DRAM timing are separate layers;
- protocol messages are not packets/flits;
- transport backpressure is not protocol RetryLater;
- fixed-latency, queued and detailed memory backends have separate fidelity claims;
- MSHR/directory/controller/router state cannot replace `TransactionKey`.

### Performance methodology

- every result has a resolved experiment manifest;
- raw events, derived metrics and conclusions remain separate;
- workload, ROI, repetitions, seeds, normalization, fidelity and exclusions are result metadata;
- analytical laws are validation partners, not substitutes for detailed models;
- Top-down views preserve causal attribution and provenance.

### Linux / virtual memory

- VMA validity, physical allocation and PTE presence are distinct states;
- guest page tables are simulated memory; TLB/walker structures are model state;
- Linux performs allocation, CoW, reclaim, migration and OOM policy inside the guest;
- Linux kernel structures are optional decoder targets, not public simulator APIs.

### RISC-V Linux machine

The first machine is the source-backed QEMU-virt-compatible subset:

```text
archlab-rv64-virt-v0
```

Linux-v0 requires one RV64 hart, M/S/U, Sv39, reset ROM, OpenSBI/SEE, DRAM at `0x80000000`, timer/software interrupt support, PLIC, NS16550 UART and a generated DTB. Linux entry is `a0=hartid`, `a1=dtb_pa`, `satp=0`. SMP, virtio, PCIe, IOMMU and AIA are later stages.

## 4. Current remaining work

### Batch 6 — R-class triage

- **43 raw records** must be resolved from R into S/A/B/C;
- opaque filenames require first-page/TOC/abstract inspection;
- newly discovered S works are promoted immediately.

### A-class milestone review

The original coarse pool contained 145 A records. Some are already absorbed into canonical batches, so the remaining canonical count is not yet trustworthy. Groups still include Virtualization, PCIe/Storage/CXL, RAS/ECC, Parallel/NUMA, Compiler/JIT, Heterogeneous, Power/Thermal, Security, Firmware and implementation-focused software engineering.

### B-class

398 raw records remain searchable and classified. They are read when a milestone or source dependency needs them; the project does not pretend that indiscriminate cover-to-cover reading is useful.

## 5. Immediate next action

1. resolve all 43 R records;
2. commit the R triage and any promoted source batches;
3. generate the normalized all-record inventory snapshot;
4. recalculate remaining A-class canonical groups;
5. continue milestone-driven L1/L2/L3 review.

Accurate current statement:

> All 623 assets have been inventoried and coarsely screened; 31 canonical source families have actual L1 review; RISC-V Linux boot contracts are source-backed; 43 ambiguous raw records are the next blocking classification task.
