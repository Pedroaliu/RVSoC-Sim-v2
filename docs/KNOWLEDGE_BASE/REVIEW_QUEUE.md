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

Completed extraction:

- functional, fixed-latency, queued and detailed-DRAM fidelity boundaries;
- cache functional state versus timing resources;
- generic transaction identity versus MSHR/directory/controller/router private state;
- consistency model versus coherence protocol;
- protocol message versus packet/flit;
- network-interface responsibility for packetization and class mapping;
- transport backpressure versus protocol `RetryLater`;
- routing, flow-control and protocol deadlock separation;
- message-level and detailed flit-level NoC backends;
- full-system timing feedback and the limits of pre-generated traces;
- layered validation from memory bytes through full-system timing.

Records:

- `reviews/batch-03-memory-coherence-noc.md`
- `topics/memory.md`
- `topics/noc.md`
- `decisions/memory-coherence-noc-boundaries.md`
- `inventory/batch-03-memory-coherence-noc.csv`

## Batch 4 — Performance / Datacenter / Methodology

Status: **next / queued**

1. The Art of Computer Systems Performance Analysis
2. Performance Modeling and Design of Computer Systems
3. The Datacenter as a Computer
4. Computer Architecture Performance Evaluation Methods

Expected extraction:

- performance-question formulation;
- metric selection and common mistakes;
- factors, levels and experiment design;
- workload selection and representativeness;
- warmup, ROI, repetitions and random seeds;
- confidence intervals and comparison methodology;
- queueing, Little’s Law and utilization/response-time relations;
- bottleneck and sensitivity analysis;
- simulator verification, validation and calibration;
- single-node versus warehouse-scale system boundaries;
- how the Top-down visualization should preserve causal attribution.

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
