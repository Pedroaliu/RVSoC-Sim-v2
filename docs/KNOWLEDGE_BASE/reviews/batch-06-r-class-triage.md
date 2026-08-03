# Batch 6 — R-class Manual Triage

> Date: 2026-08-03  
> Scope: resolve every raw record previously marked `R` using title/metadata and, for opaque filenames, actual first-page/TOC/abstract inspection.

## 1. Result

- Raw R records processed: **43**
- Canonical groups formed: **32**
- Promoted to A: **12 raw records / 11 canonical groups**
- Retained as B: **31 raw records / 21 canonical groups**
- Promoted to S: **0**
- Excluded as C: **0**
- Remaining R records: **0**

The absence of S promotions is a result, not a quota: the opaque records contained useful architecture, Linux, workload, concurrency and RAS material, but none displaced the already selected S-level primary sources.

## 2. Important corrections discovered

### K0038 — `understand.pdf`

Actual title: **Understanding the Linux Virtual Memory Manager** by Mel Gorman. It is retained as A for historical Linux VM implementation and debugging methodology, with an explicit warning that Linux 2.4/2.6 implementation details are not modern-kernel facts.

### K0312 — truncated/misleading filename

Actual first pages identify **Essentials of Computer Architecture, Second Edition** by Douglas Comer (2017), not a generic programming-foundations book. It is promoted to A.

### K0276/K0278/K0279/K0280

These are volumes of the **Visualize-ML / 鸢尾花书** series: programming, visualization, data analysis and machine learning. They are one related series, not four unrelated opaque files.

### K0367–K0374

These are numbered historical-perspective/further-reading section supplements from a computer-organization textbook family. They are grouped as one related canonical source family rather than seven books.

### K0514 — `wlmod.pdf`

Actual title: **Workload Modeling for Computer Systems Performance Evaluation** by Dror G. Feitelson. It is promoted to A because it covers representativeness, data collection/cleaning, distributions, heavy tails, locality and workload correlation—directly useful for ArchLab trace and synthetic traffic generation.

### Generic paper filenames

- `002_2014_HPDC14.pdf` → **Improving Energy Efficiency of Embedded DRAM Caches for High-end Computing Systems (ESTEEM)**.
- `003_2016_DataClound16.pdf` → **An Efficient Parallel Implementation of a Light-weight Data Privacy Method for Mobile Cloud Users**.
- `001_2014_GLVSL14.pdf` → **WriteSmoothing: Improving Lifetime of Non-volatile Caches Using Intra-set Wear-leveling**.

## 3. A-level promotions

- **R-QEMU-EMBED-001 — How to Develop Embedded Software Using the QEMU Machine Emulator**  
  Module: QEMU frontend / device models / virtual platform bring-up; review at: QEMU integration milestone.
- **R-LINUX-VM-001 — Understanding the Linux Virtual Memory Manager**  
  Module: Linux-aware observation / historical VM implementation / page allocator; review at: Linux MM analysis and decoder milestone.
- **R-ERROR-CONTROL-CODING-001 — Error Control Coding, Second Edition**  
  Module: ECC / coding theory / memory and link reliability; review at: RAS/ECC milestone.
- **R-COMER-ARCH-001 — Essentials of Computer Architecture, Second Edition (Douglas Comer)**  
  Module: Architecture fundamentals / processor / memory / I/O / buses; review at: Architecture foundation and teaching support.
- **R-NULL-ARCH-001 — The Essentials of Computer Organization and Architecture (Linda Null et al.)**  
  Module: Architecture and organization foundation; review at: Architecture foundation and teaching support.
- **R-STALLINGS-COA11-001 — Computer Organization and Architecture: Designing for Performance, 11th Edition**  
  Module: Architecture / memory / I/O / parallel systems foundation; review at: Architecture foundation and cross-checking.
- **R-POWER-EFFICIENT-ARCH-001 — Computer Architecture Techniques for Power-Efficiency**  
  Module: Power/energy models and architecture techniques; review at: Power/thermal milestone.
- **R-HARDWARE-DEPENDENT-SW-001 — Hardware-dependent Software: Principles and Practice**  
  Module: Hardware/software interface / firmware / model-driven development; review at: Firmware and platform-integration milestone.
- **R-NONBLOCKING-SYNC-001 — Practical Implementations of Non-Blocking Synchronization Primitives**  
  Module: Atomic primitives / lock-free algorithms / memory ordering; review at: Multicore and concurrency milestone.
- **R-WORKLOAD-MODELING-001 — Workload Modeling for Computer Systems Performance Evaluation**  
  Module: Workload collection / characterization / synthetic traffic / statistical models; review at: Stats, synthetic workload and experiment infrastructure.
- **R-CPU-GPU-FAULT-DETECTION-001 — Evaluating the Viability of Application-Driven Cooperative CPU-GPU Fault Detection**  
  Module: Heterogeneous RAS / fault detection / CPU-GPU cooperation; review at: Heterogeneous RAS milestone.

## 4. Canonicalization results

The 43 raw records collapse into 32 canonical groups. Multi-record groups include:

- four Visualize-ML volumes grouped as one related series;
- seven chapter-history supplements grouped as one textbook supplement family;
- two Professional C++ records retained as probable edition/copy variants;
- two Linda Null architecture records retained as probable title/edition variants.

Canonicalization never deletes a raw record. Edition, page-count and byte-hash checks remain required before declaring byte-identical duplicates.

## 5. Integrity limits

- `Title/metadata` classification is not represented as an L1 reading.
- `First pages/TOC/abstract inspected` means enough content was read to identify and classify the work, not full detailed review.
- Duplicate relationships remain `probable` until edition/page-count/hash checks are complete.
- The full normalized inventory preserves every raw record; canonicalization does not delete files.

## 6. Next action

With R eliminated, the next work is:

1. use the normalized 623-record inventory as the durable source of classification truth;
2. recalculate outstanding A-level canonical groups instead of relying on the old raw count;
3. review A groups by ArchLab milestone;
4. read B sources on demand through topic/citation gaps.
