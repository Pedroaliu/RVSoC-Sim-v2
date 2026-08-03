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

Completed extraction:

- experiment-question and system-boundary formulation;
- workload selection and representativeness;
- metric, baseline, averaging and ratio pitfalls;
- factors, levels, repetitions, seeds and confidence;
- functional, trace-driven, execution-driven and full-system evaluation scopes;
- analytical, empirical and hybrid models;
- Little's Law and queueing bounds as simulator sanity checks;
- simulator verification, validation and calibration;
- run metadata and result provenance;
- warehouse-scale workload, monitoring, tail, power, availability and cost scope;
- Top-down causal path from service objective to cycle-level events.

Records:

- `reviews/batch-04-performance-datacenter-methodology.md`
- `topics/performance.md`
- `decisions/performance-evaluation-methodology.md`
- `inventory/batch-04-performance-methodology.csv`

## Batch 5 — Linux MM / full-system integration

Status: **next / queued**

Primary sources:

1. The Linux Memory Manager — Lorenzo Stoakes
2. Operating Systems: Three Easy Pieces / OSTEP family
3. Architectural and Operating System Support for Virtual Memory
4. Linux NUMA and local/remote-memory papers in the inventory
5. Supporting Linux kernel architecture and virtual-memory references as needed

Expected extraction:

- software-visible physical/virtual-memory behavior required by a functional platform;
- page-table walk, TLB, permission and fault boundaries;
- Sv39/MMU responsibilities versus Linux memory-management policy;
- demand paging, CoW, mmap, page faults and anonymous/file-backed memory;
- NUMA placement, local/remote access and migration;
- timer, interrupt, UART, DTB and boot contracts needed before Linux;
- which Linux behaviors require detailed timing and which require only functional correctness;
- full-system validation from OpenSBI to BusyBox shell;
- observation points for VM/process/page/transaction Top-down analysis.

## Batch 6 — R-class manual triage

Status: **43 raw records queued**

For each R item:

1. identify bibliographic title from the first pages;
2. inspect table of contents or abstract;
3. assign canonical work and domain;
4. decide S/A/B/C;
5. record whether the filename was opaque, truncated or misleading;
6. immediately promote any newly discovered S source into its own review batch.

The R batch comes before broad A-class review so an important source is not lost merely because its filename was poor.

## A-class milestone review

Status: **145 raw records queued; canonical count unknown until deduplication**

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
