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

Completed extraction:

- architecture versus microarchitecture;
- functional, timing in-order and detailed OOO model boundaries;
- fetch, prediction, decode, rename, allocation, ROB, issue, execution, LSQ, commit and recovery responsibilities;
- instruction/register/memory flow performance hierarchy;
- precise exception and speculative-state recovery requirements;
- CPU-to-memory transaction boundary;
- SoC interconnect, abstraction-level and transactor implications;
- validation split between ISA correctness, pipeline invariants, speculation correctness and timing calibration.

Records:

- `reviews/batch-02-cpu-isa-ooo.md`
- `topics/cpu.md`
- `inventory/batch-02-cpu-isa-ooo.csv`

## Batch 3 — Memory / Cache / Coherence / NoC

Status: **next / queued**

1. Memory Systems: Cache, DRAM, Disk
2. The Memory System
3. A Primer on Memory Consistency and Cache Coherence, 2nd ed.
4. On-Chip Networks, 2nd ed.
5. GARNET paper

Expected extraction:

- request path, ownership and completion;
- cache lookup, fill, eviction, writeback and transient state;
- primary/secondary misses and MSHR lifecycle;
- memory consistency versus cache coherence;
- protocol state machine versus NoC transport;
- packet/flit, router, VC, buffer, credit, arbitration, QoS and deadlock;
- DRAM-controller/backend boundary;
- which parts belong in generic transactions and which remain subsystem-private.

## Batch 4 — Performance / Datacenter / Methodology

Status: **queued**

1. The Art of Computer Systems Performance Analysis
2. Performance Modeling and Design of Computer Systems
3. The Datacenter as a Computer

Expected extraction:

- experiment design, calibration and confidence;
- queueing and Little's Law;
- bottleneck attribution;
- workload selection and representativeness;
- system-level and warehouse-scale performance scope.

## Batch 5 — Linux MM / full-system integration

Status: **queued**

1. The Linux Memory Manager
2. OSTEP / Operating System Concepts family as supporting references
3. Virtual-memory and Linux NUMA papers from the inventory

Expected extraction:

- page-table and fault behavior needed by the functional platform;
- Sv39/MMU boundary;
- software-visible timer, interrupt and device contracts;
- Linux boot validation and observability.

## Batch 6 — R-class manual triage

Status: **43 records queued**

For each R item:

1. identify bibliographic title from the first pages;
2. inspect table of contents or abstract;
3. assign canonical work and domain;
4. decide S/A/B/C;
5. record whether the filename was opaque, truncated or misleading.

## Later milestone-driven groups

- Virtualization / QEMU / KVM / IOMMU
- PCIe / NVMe / SSD / CXL
- RAS / ECC / Chipkill / fault injection / recovery
- Parallel / NUMA / HPC
- Compiler / assembly / JIT / binary translation
- GPU / NPU / DPU / SIMT / collective communication
- Power / thermal / energy
- Security / confidential computing

These groups remain in the inventory now, but detailed reading starts when their ArchLab milestone approaches, unless a source affects a current foundational contract.
