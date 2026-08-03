# Batch 7 — A-level Canonicalization and Remaining Review Set

> Date: 2026-08-03  
> Scope: recalculate A-level work from the normalized 623-record inventory after R-class resolution. This is canonicalization and priority correction, not blanket L1 reading.

## 1. Result

- Raw records after correction: **623**
- A raw records: **156**
- A canonical groups: **121**
- A groups already covered by L1 batches: **7 groups / 22 raw records**
- Remaining A groups not yet at L1: **114 groups / 134 raw records**
- R records: **0**
- B raw records: **429**
- C raw records: **2**

The old `145 A records` number is retired. It was a pre-canonicalization raw count and did not subtract sources already covered in Batches 1–5B. The working review set is now **114 remaining A canonical groups**.

## 2. Content-backed corrections made during canonicalization

### `hardwareVirt.pdf`
Resolved title: **Hardware Support for Efficient Virtualization**. It remains A and is queued for the virtualization batch.

### `PMU_DATA.pdf`
Resolved title: **Monitoring of the Performance of Excitation System Based on Measured PMU Data**. Here PMU means a power-grid phasor measurement unit, not a processor performance monitoring unit. It is demoted from A to C for current ArchLab scope.

### `microarchitecture.pdf`
Resolved title: **The Microarchitecture of Intel, AMD, and VIA CPUs** by Agner Fog, updated 2025-07-25. It remains A as a detailed implementation/measurement cross-check, not as an ISA contract.

### `2210.14324v1.pdf`
Resolved title: **The Championship Simulator: Architectural Simulation for Education and Competition**. It is reclassified from CPU-primary to Simulation & Modeling and queued for the trace-driven/ChampSim comparison stage.

### `2014_paper_microcode*.pdf`
Resolved family: **Security Analysis of x86 Processor Microcode**. The two files are grouped as a probable duplicate/version pair and moved to Security / Trusted Computing.

### RISC-V organization textbook files
`Computer_Organization_RiscV_Edition_5th...` and `HandP_RISCV.pdf` are one canonical family: **Computer Organization and Design: The Hardware/Software Interface, RISC-V Edition**. Four raw files collapse to one group.

## 3. Remaining A groups by domain

- Memory / Cache / DRAM / NVM / CXL: **36**
- RAS / Reliability / ECC: **18**
- Simulation & Modeling: **11**
- OS / Linux / Kernel / Virtual Memory: **11**
- CPU / ISA / Microarchitecture: **8**
- Parallel / HPC / Concurrency / NUMA: **8**
- Virtualization / QEMU / KVM / IOMMU: **6**
- Storage / PCIe / NVMe / SSD / USB: **6**
- Performance / Datacenter / Benchmarking: **3**
- AI / GPU / NPU / Heterogeneous: **2**
- Security / Trusted Computing: **2**
- Firmware / Boot / UEFI / BIOS: **1**
- NoC / Interconnect / Networking: **1**
- RTL / Digital Logic / FPGA: **1**

## 4. Multi-record canonical groups still requiring duplicate/edition verification

- **A-COD-RISCV — Computer Organization and Design: The Hardware/Software Interface, RISC-V Edition**: K0337;K0342;K0375;K0376; same canonical book; two filename families and duplicate copies; confidence `high`.
- **A-RISCV-ASM-SMITH — RISC-V Assembly Language Programming (Stephen Smith)**: K0246;K0247; probable byte duplicate; confidence `high`.
- **A-NULL-COA — The Essentials of Computer Organization and Architecture**: K0323;K0334; probable duplicate/title variants; confidence `high`.
- **A-EASYCRASH — EasyCrash: Exploring Non-Volatility of NVM for HPC under Failures**: K0565;K0575; title/version variants; confidence `medium`.
- **A-MULTICORE-CACHE — Multi-Core Cache Hierarchies**: K0378;K0406; probable duplicate/title typo variants; confidence `high`.
- **A-CXL-PERF-CHAR — Performance Characterization of CXL Memory and Its Use Cases**: K0530;K0531; byte duplicate candidate; confidence `high`.
- **A-GPU-HETMEM-PLACEMENT — Performance Modeling for Optimal Data Placement on GPU with Heterogeneous Memory Systems**: K0588;K0600; related/possibly different versions; confidence `medium`.
- **A-UNIMEM — Unimem: Runtime Data Management on NVM-based Heterogeneous Main Memory**: K0582;K0595; title/version variants; confidence `high`.
- **A-NOC-ROUTER-MICROARCH — Microarchitecture of Network-on-Chip Routers**: K0123;K0330; probable byte duplicate; confidence `high`.
- **A-OSIDP — Operating Systems: Internals and Design Principles**: K0068;K0074; edition/title variants; confidence `medium`.
- **A-LINUX-KERNEL-3 — Understanding the Linux Kernel, Third Edition**: K0049;K0077; probable byte duplicate; confidence `high`.
- **A-LINUX-VM-GORMAN — Understanding the Linux Virtual Memory Manager**: K0038;K0048; same canonical work/title variant; confidence `high`.
- **A-PERFBOOK — Is Parallel Programming Hard, And If So, What Can You Do About It?**: K0458;K0461;K0502; different released drafts/editions; confidence `high`.
- **A-FAULT-TOL-ARCH — Fault-Tolerant Computer Architecture**: K0091;K0393; probable byte duplicate; confidence `high`.
- **A-FUND-ECC — Fundamentals of Error-Correcting Codes**: K0209;K0335; probable duplicate/title variants; confidence `high`.
- **A-DIGITAL-LOGIC-VERILOG — Fundamentals of Digital Logic with Verilog Design, Third Edition**: K0296;K0322; probable byte duplicate; confidence `high`.
- **A-MICROCODE-SECURITY — Security Analysis of x86 Processor Microcode**: K0364;K0366; probable duplicate/version pair; confidence `high`.

## 5. Covered A groups

- **ARCH-001 — Computer Architecture: A Quantitative Approach, Sixth Edition**: K0350;K0354;K0355;K0356;K0357;K0358;K0359;K0360;K0361;K0362;K0363; covered by Batch 2.
- **COH-001 — A Primer on Memory Consistency and Cache Coherence**: K0416; covered by Batch 3.
- **VM-001 — Architectural and Operating System Support for Virtual Memory**: K0387;K0398; covered by Batch 5.
- **NUMA-001 — Local and Remote Memory in a Linux/NUMA System**: K0071; covered by Batch 5.
- **OS-001 — Operating Systems: Three Easy Pieces**: K0061;K0063; covered by Batch 5.
- **PERF-004 — Computer Architecture Performance Evaluation Methods**: K0391; covered by Batch 4.
- **SIM-001 — Fundamentals of Full-Platform Simulation**: K0141;K0148;K0150;K0152; covered by Batch 1.

## 6. Durable inventory files

- Full normalized v3 CSV: Drive ID `1Rcm-ivqexORPxfz07ThJe_-afTGES361`
- Full normalized v3 SHA-256: `61388baed6db3474fefcaddb362d77647297ef404e864f0757b9159b08acd150`
- A canonical groups v1 CSV: Drive ID `1VwbkcSjDeJBZ070M-joYdt3XuEHBil6F`
- A canonical groups v1 SHA-256: `5b89e81fff4010bbff00708ea41a4206ff3878ec4b1c8d7c111160ca0158865b`

## 7. Integrity limits

- A canonicalization is title/metadata based unless the row explicitly records content inspection.
- `not-yet-L1` means the work has not received a Batch-style content review.
- `probable duplicate` is not byte-identity proof.
- The 114-group count is the actionable review queue, but it may shrink slightly as later content inspection merges additional editions or title variants.
- Priority may still change after L1 review; A means worth milestone review, not guaranteed adoption.

## 8. Next batch

Batch 8 is **Virtualization / QEMU / KVM / IOMMU**. It is first because it directly connects the current functional platform, `archlab-rv64-virt-v0`, QEMU integration, ArchLab-virt, H-extension and future PCIe/IOMMU work.
