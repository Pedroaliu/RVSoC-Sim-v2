# Detailed Review Batch 10 — RAS, ECC, Fault Injection, and Recovery

> Review level: L1 — actual TOC, abstract/introduction and selected architecture/method sections inspected.  
> Date: 2026-08-03

Batch 10 closes all 18 not-yet-L1 A canonical groups in the RAS/ECC domain. It does not claim transistor-level failure-rate prediction or vendor-exact Chipkill geometry.

## Sources reviewed

### 1. A-CAN-051 — Characterization and Comparison of Application Resilience for Serial and Parallel Codes

- **Raw records:** K0592
- **Verified scope:** Application-level bit-flip injection on serial and MPI versions of NAS CG/FT/BT; benign, SDC and crash outcomes; comparison of common and parallel-only vulnerable instruction sites; sample-count stabilization.
- **ArchLab use:** Workload-sensitive fault-injection campaigns and serial-vs-parallel resilience comparisons.
- **Caution:** Preliminary poster with small process count and selected instructions; not a hardware fault-rate model.
- **Priority after review:** A
- **Source:** Drive ID 1tXcIpIR1FW9yeQySkG9D_e5TVrpPVzMd

### 2. A-CAN-052 — Classifying Soft Error Vulnerabilities in Extreme-Scale Scientific Applications Using a Binary Instrumentation Tool

- **Raw records:** K0618
- **Verified scope:** BIFIT object-specific and execution-point-specific bit-flip injection; transient versus hard faults; success, abort and SDC outcomes; per-data-object vulnerability classification and selective protection.
- **ArchLab use:** Semantic fault injection into named data objects and explicit outcome taxonomy.
- **Caution:** Emulates application-visible bit corruption after hardware masking; does not model the complete physical fault path.
- **Priority after review:** A
- **Source:** Drive ID 1FuQbe6eiKfGzbwchHloDd7ZQAgUHiQt9

### 3. A-CAN-053 — Fast Fault Injection and Sensitivity Analysis for Collective Communications

- **Raw records:** K0603
- **Verified scope:** FastFIT for MPI collectives; faults in collective parameters and buffers; success/application-detected/MPI-error/segfault/wrong-answer/infinite-loop outcomes; semantic and context pruning plus ML prediction.
- **ArchLab use:** Collective-communication fault model, propagation experiments and campaign-space reduction.
- **Caution:** Focuses correctness sensitivity of MPI collectives, not recovery or physical interconnect fault timing.
- **Priority after review:** A
- **Source:** Drive ID 1LqmXccBz_h4ULNVN-2d5tZymt7o3GvcN

### 4. A-CAN-054 — FlipTracker: Understanding Natural Error Resilience in HPC Applications

- **Raw records:** K0578
- **Verified scope:** Code-region model, dynamic data-dependency graph and alive-corrupted-location tracking; six natural-resilience computation patterns; verification success/failure/crash and propagation analysis.
- **ArchLab use:** Error-propagation observability and application-level masking analysis.
- **Caution:** Natural resilience is workload and implementation dependent; it must not silently weaken architectural protection guarantees.
- **Priority after review:** A
- **Source:** Drive ID 11zTnxjYUozm7fXRsspVtP8X1TBglgz55

### 5. A-CAN-055 — MATCH: An MPI Fault Tolerance Benchmark Suite

- **Raw records:** K0576
- **Verified scope:** Structured comparison of MPI fault-tolerance designs using proxy applications; checkpointing with FTI and process recovery with restart, ULFM and Reinit; data-dependency analysis for checkpoint selection.
- **ArchLab use:** Recovery benchmark suite, checkpoint/restart state selection and overhead decomposition.
- **Caution:** Targets fail-stop process/node failure and MPI recovery, not silent hardware corruption.
- **Priority after review:** A
- **Source:** Drive ID 1yQU84wyhe8nAJN_7Iyh9Tofr7DQL9OXw

### 6. A-CAN-056 — Quantitatively Modeling Application Resilience with the Data Vulnerability Factor

- **Raw records:** K0610
- **Verified scope:** DVF metric combines hardware FIT, execution time, object size and hardware-access exposure; Aspen-based analytical modeling; data-structure-level vulnerability and performance/resilience tradeoffs.
- **ArchLab use:** Fast analytical cross-check for detailed fault injection and AVF-style metrics.
- **Caution:** DVF is a modeling abstraction with assumptions; it cannot replace fault-path simulation or empirical calibration.
- **Priority after review:** A
- **Source:** Drive ID 1xbaIVhVIwdFckhDNGSw2wb0-TclqZHlF

### 7. A-CAN-057 — Rethinking Algorithm-Based Fault Tolerance with a Cooperative Software-Hardware Approach

- **Raw records:** K0614
- **Verified scope:** Coordination of application ABFT and memory ECC; selective/relaxed ECC, error notification and software-hardware co-design; SECDED and symbol-oriented Chipkill context.
- **ArchLab use:** Cross-layer policy experiments where application semantics and hardware protection cooperate.
- **Caution:** Adaptive protection is an optional research policy, never the default correctness contract.
- **Priority after review:** A
- **Source:** Drive ID 1NxLDWh5aRI7GBri4vBUzn6v_NBCt0Hpg

### 8. A-CAN-058 — Understanding Ineffectiveness of the Application-Level Fault Injection

- **Raw records:** K0596
- **Verified scope:** Sensitivity of random fault-injection conclusions to campaign size and input problem; LLVM FlipIt tests across seven NAS benchmarks and 1,000–10,000 injections.
- **ArchLab use:** Campaign manifest, convergence checks, repetitions and confidence requirements.
- **Caution:** A single injection count or workload is not a universal statistical rule.
- **Priority after review:** A
- **Source:** Drive ID 1kITCVLPu9va66vKkqy7l9Bq5dTGp0Sot

### 9. A-CAN-059 — Coding Theory: Algorithms, Architectures, and Applications

- **Raw records:** K0211
- **Verified scope:** Linear block codes, generator/parity-check matrices and syndrome; cyclic/BCH/Reed-Solomon codes; convolutional/Viterbi, LDPC, turbo codes; algorithms and implementation concerns.
- **ArchLab use:** Reusable ECC encoder/decoder interfaces and algorithm references for memory, cache, links and storage.
- **Caution:** Communication-channel examples are broader than ArchLab memory RAS; code choice must follow component geometry and fault model.
- **Priority after review:** A
- **Source:** Drive ID 1wdqA-FySHrUMumezOsm1Qyw69GSEwDCY

### 10. A-CAN-060 — DRAM Errors in the Wild: A Large-Scale Field Study

- **Raw records:** K0101
- **Verified scope:** Large production-fleet DRAM field data over 2.5 years; CE/UE incidence, hard-versus-soft behavior, DIMM technology/age/temperature/utilization factors, SECDED/Chipkill context and memory scrubbing.
- **ArchLab use:** Primary empirical basis for DRAM fault distributions, persistent-fault modeling, CE history, scrub and retirement experiments.
- **Caution:** 2006–2008 fleet and older DDR generations; use qualitative distributions and methodology, then calibrate modern systems separately.
- **Priority after review:** S
- **Source:** Drive ID 1i0Dz6j4muscxG-7WGiDKl9a8XVypCKIC

### 11. R-ERROR-CONTROL-CODING-001 — Error Control Coding, Second Edition

- **Raw records:** K0212
- **Verified scope:** Linear block codes, Hamming SEC/SECDED, cyclic/BCH/Reed-Solomon, errors and erasures, convolutional/Viterbi and advanced iterative codes with encoding/decoding implementation.
- **ArchLab use:** Canonical coding algorithms, syndrome generation and test-vector construction.
- **Caution:** The full text is communications-oriented and much broader than the initial memory-ECC subset.
- **Priority after review:** A
- **Source:** Drive ID 1WbroQy5uQhmwtdol_gMnZxIQFkIlMd0F

### 12. R-CPU-GPU-FAULT-DETECTION-001 — Evaluating the Viability of Application-Driven Cooperative CPU/GPU Fault Detection

- **Raw records:** K0617
- **Verified scope:** Selective data protection for GPU global memory, CPU/GPU cooperative detection, single- and multi-bit injection and application-semantic vulnerability analysis.
- **ArchLab use:** Heterogeneous RAS policy, CPU/GPU cooperation and accelerator-memory fault campaigns.
- **Caution:** Application-specific study on one workload; detection coverage and overhead are not general GPU guarantees.
- **Priority after review:** A
- **Source:** Drive ID 1yEMCkKAplnMcJSXKqlVQINhAJBwwNm-r

### 13. A-CAN-061 — Fault-Tolerance Techniques for SRAM-Based FPGAs

- **Raw records:** K0086
- **Verified scope:** Radiation and SEU effects, EDAC, TMR, architectural and high-level mitigation, scrubbing, bitstream fault injection, golden-chip methodology, area/performance/coverage evaluation.
- **ArchLab use:** RTL/FPGA co-simulation fault injection, TMR/scrub state machines and coverage methodology.
- **Caution:** Focuses SRAM FPGA/radiation and older device families; do not generalize directly to CPU/DRAM field failures.
- **Priority after review:** A
- **Source:** Drive ID 1p1h1R_jxjEP7IpZDW9Y8-x2HHZW-0MHK

### 14. A-FAULT-TOL-ARCH — Fault Tolerant Computer Architecture

- **Raw records:** K0091;K0393
- **Verified scope:** Fault-error-failure chain, masking and AVF; transient/permanent/intermittent models; error detection, forward/backward recovery, diagnosis and self-repair across cores, caches, memory, coherence and interconnects.
- **ArchLab use:** Primary architectural RAS decomposition and fault/recovery state-machine reference.
- **Caution:** 2009 survey; mechanisms and examples need current implementation-specific validation.
- **Priority after review:** S
- **Source:** Drive IDs 1UnNAZgg3qbDB9T2FT5oG32a3xg8gpAwN;1lg6uMUFjvzroWxC_fBk-XxQ4PAw6gdJh

### 15. A-CAN-062 — Fault-Tolerant Systems, Second Edition

- **Raw records:** K0098
- **Verified scope:** Fault classification, reliability/availability/MTTF and Markov analysis; hardware and information redundancy, coding, RAID/replication/ABFT, resilient networks, software fault tolerance, recovery and distributed-system techniques.
- **ArchLab use:** Primary system-level dependability taxonomy and analytical/recovery reference.
- **Caution:** Very broad; ArchLab should extract only mechanisms tied to modeled components and explicit experiments.
- **Priority after review:** S
- **Source:** Drive ID 1A0bNiH9g6TrAqGkuEIDzyEcvG8FkyIjl

### 16. A-FUND-ECC — Fundamentals of Error-Correcting Codes

- **Raw records:** K0209;K0335
- **Verified scope:** Mathematical foundation of linear codes, distance and bounds; finite fields; cyclic/BCH/Reed-Solomon; burst errors/interleaving; convolutional, soft-decision, LDPC and turbo codes.
- **ArchLab use:** Mathematical reference and independent validation of code capabilities and decoder behavior.
- **Caution:** Theory-heavy; not the first implementation guide for simulator components.
- **Priority after review:** A
- **Source:** Drive IDs 1Yn1GJVX64uK9MbLjUt7RKRyTKRU7GR6Z;1oPvsRRI68TJIUH9XguiJb0VCBQNshgwX

### 17. A-CAN-063 — Design and Analysis of an Adjacent Multi-bit Error Correcting Code for Nanoscale SRAMs

- **Raw records:** K0207
- **Verified scope:** Adjacent multi-bit upset model for nanoscale SRAM, SECDED/BCH/RS comparisons, parity-check construction, encoder/decoder area and delay, corrected-SER modeling and neutron irradiation validation.
- **ArchLab use:** Spatial/burst fault geometry, adjacent-bit ECC and SRAM implementation tradeoff experiments.
- **Caution:** SRAM-specific proposed code and technology assumptions; not a DRAM Chipkill specification.
- **Priority after review:** A
- **Source:** Drive ID 1A9BxB4L51Ldrai4MqwuQdMz0pqE-9Ezp

### 18. A-CAN-064 — The Art of Error Correcting Coding, Second Edition

- **Raw records:** K0214
- **Verified scope:** Practical introduction to Hamming/Golay/Reed-Muller, cyclic/BCH/Reed-Solomon, convolutional/Viterbi, product/concatenated, soft-decision and modern coding techniques.
- **ArchLab use:** Concise coding-algorithm cross-reference and exercises/test vectors.
- **Caution:** Communications focus; use as supporting source rather than component-level RAS contract.
- **Priority after review:** A
- **Source:** Drive ID 1aL2JlaD9SsO27JyCjLZagfUWiX6dP0qB

## Cross-source conclusions

### 1. Fault, error, failure, report, and recovery are different objects

```text
physical fault
    ↓ may be masked
microarchitectural error
    ↓ may be detected/corrected/masked
architectural corruption or contained event
    ↓
software-visible report / machine check / interrupt / log
    ↓
recovery, repair, retirement, deconfiguration, or service action
```

An injected fault must not automatically create a detected error or a software-visible failure. Every transition is explicit and independently observable.

### 2. RAS state preserves location and geometry

Fault records retain component, hierarchy and physical organization: core/unit/register/cache-set/way, NoC router/link/VC, PCIe function/link, memory controller/channel/subchannel/rank/device/bank/row/column/beat/symbol/bit. ECC capability is derived from configurable organization, not from a hard-coded “Chipkill=true” flag.

### 3. ECC is a component with a contract

An ECC model defines codeword layout, symbol width, protected data, check bits, encode/check/correct latency, syndrome, detected/corrected/uncorrectable/miscorrected outcomes and error-location information. SECDED, adjacent-bit codes, BCH/Reed–Solomon and symbol-oriented Chipkill-style schemes are separate policies behind one interface.

### 4. Scrub, retry, repair, and retirement are explicit events

Periodic/patrol scrub, read-triggered correction writeback, retry, cache-line delete, page retirement, device/rank deconfiguration and spare substitution have independent timing and state. CE history and thresholds belong to policy; the raw syndrome/event stream remains immutable evidence.

### 5. Fault injection campaigns require a resolved manifest

Each campaign records fault model, target population, spatial/temporal distribution, seed, injection time, workload/input, ROI, repetitions, stopping/convergence rule, expected confidence, detection point and outcome taxonomy. Random injection count is never accepted without a convergence check.

### 6. Outcome metrics remain separate

At minimum: masked, corrected, detected-unrecovered, contained, crash/hang, SDC, wrong answer within/outside tolerance, recovery success, recovery latency, performance overhead and service impact. AVF/DVF, SDC rate, CE/UE incidence, MTTF/MTBF and availability are not interchangeable.

### 7. Application-aware resilience is optional policy

Natural resilience, ABFT and selective protection are useful research modes. They may reduce overhead only when explicitly enabled and validated. The default architectural model cannot assume an application will mask corruption.

### 8. POWER-inspired layered RAS remains the long-term system target

ArchLab should ultimately model coordinated hardware, firmware, OS and service-processor behavior: detect → classify → contain → recover → diagnose → isolate/retire → report/service. Batch 10 establishes the contracts; it does not move RAS ahead of the current M0.3/M0.4/functional-Linux implementation sequence.

## Initial implementation ladder

```text
RAS-0  Deterministic fault object + one-shot injection + event log
RAS-1  SECDED component + syndrome + CE/UE outcomes
RAS-2  Scrub, CE history, thresholds and page/line retirement
RAS-3  Configurable symbol/device geometry + Chipkill-style policies
RAS-4  Core/cache/NoC/PCIe fault propagation and containment
RAS-5  Firmware/OS/service-processor recovery and deconfiguration
RAS-6  AVF/DVF/MTTF experiments and application-aware policies
```

## Progress

- L1 source families before Batch 10: 44
- added in Batch 10: 18
- total L1 source families: **62**
- A canonical groups after Batch 10: **38 covered / 83 not-yet-L1**
- RAS / Reliability / ECC A domain: **fully L1-covered**

## Inventory integrity

- normalized raw snapshot v6 SHA-256: `d2a6767a8361985bde98176bdf4cf2feee91ea5f35957d873ed5ca59167d532c`
- A canonical coverage v4 SHA-256: `cc971d4e1dd338e45bb658513e4af9bfcf219f914ca6f499c769c4ba955d750b`
- Batch 10 inventory SHA-256: `bd33a11b5f3438e85b5cc6907584c114701619bd92478b6138ce183a06b0f289`
