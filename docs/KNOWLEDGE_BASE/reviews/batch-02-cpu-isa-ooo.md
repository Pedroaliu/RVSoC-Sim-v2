# Detailed Review Batch 2 — CPU, ISA, OOO and SoC Boundaries

> Review level: **L1**  
> Date: 2026-08-03

L1 means that the actual table of contents, front matter, introduction and selected architecture sections were inspected. It does not mean cover-to-cover reading. Detailed algorithms and implementation extraction will be promoted to L2/L3 when the corresponding ArchLab milestone begins.

## 1. Canonical works reviewed

### CPU-001 — Modern Processor Design: Fundamentals of Superscalar Processors

- **Authors:** John Paul Shen and Mikko H. Lipasti
- **Edition inspected:** 2005 text, reissued by Waveland Press in 2013
- **Raw inventory records:** `K0316` Chinese translation; `K0318` English edition
- **Canonical relation:** same work in different languages/editions; not treated as byte-identical duplicates
- **Review level:** L1
- **Priority after review:** S

#### Verified scope

The book is organized as a systematic path from processor design principles to increasingly aggressive execution:

1. processor design, the architecture/implementation/realization distinction, the dynamic-static interface, the processor performance equation, performance evaluation and ILP limits;
2. pipelined processor design, stage balancing and stall minimization;
3. memory hierarchy, virtual memory, buses and I/O;
4. superscalar organization with fetch, decode, dispatch, execute, complete and retire;
5. instruction-flow, register-data-flow and memory-data-flow techniques;
6. PowerPC 620 as a trace-driven performance-analysis case;
7. Intel P6 as a concrete in-order-front-end/out-of-order-core/retirement design;
8. superscalar history, classification and verification;
9. advanced branch and fetch techniques;
10. value locality, reuse and value prediction;
11. coherence, consistency, CMP, fine/coarse-grained multithreading and SMT.

The book’s “iron law” treatment is useful because it refuses to optimize instruction count, CPI or cycle time in isolation: reducing one term may worsen another. Its superscalar template makes a second critical distinction: instructions may execute and finish out of order, but architectural state must be updated in program order to retain precise exceptions.

The three-flow framework is particularly useful for ArchLab:

```text
instruction flow  → fetch, prediction, control recovery
register data flow → dependencies, rename, scheduling, execution
memory data flow   → address, ordering, forwarding and cache interaction
```

The PowerPC 620 case also demonstrates both the value and the limits of trace-driven evaluation: the trace model can quantify stalls and IPC, but speculative instructions absent from the trace and simplified variable-latency operations reduce fidelity.

#### ArchLab value

This is the best Batch 2 source for the **conceptual organization and performance reasoning** of a superscalar processor. It explains why the structures exist and how their bottlenecks interact before diving into a particular physical implementation.

Direct uses:

- define the future timing-core pipeline and stage responsibilities;
- separate instruction, register and memory flow bottlenecks in statistics;
- structure OOO validation around precise state, speculation and recovery;
- understand SMT as sharing and scheduling resources, not merely duplicating threads;
- design performance experiments that expose fetch, dispatch, issue, execution and retirement stalls.

#### Do not copy literally

- The text and product case studies reflect a 2005-era design space.
- Specific widths, predictor sizes, cache assumptions and PowerPC/P6 organizations are examples, not project defaults.
- Trace-driven results must not be presented as full-system or wrong-path-accurate results.
- Advanced value prediction and implicit multithreading are research extensions, not prerequisites for the first OOO core.

#### Targeted reading plan

- **Before timing in-order core:** Chapters 1, 2 and 4.
- **Before first OOO core:** Chapter 5 and the relevant parts of Chapter 7.
- **Before SMT:** Chapter 11.
- **For validation methodology:** Chapter 6 and Section 8.4.
- **Later research:** Chapters 9 and 10.

#### Source locators

- English: `Modern Processor Design_ Fundamentals of Superscalar Processors - PDF Room(20220908093341).pdf`
- Chinese: `现代处理器设计 (舍恩, 利帕斯蒂) (Z-Library).pdf`
- Drive English ID: `1p888MDXAeexoB8c6kcLt59aPLB2m8g54`
- Drive Chinese ID: `126Txh7ruQpN18X9TE7HhQlGHSoJNWteH`

---

### ARCH-001 — Computer Architecture: A Quantitative Approach, Sixth Edition

- **Authors:** John L. Hennessy and David A. Patterson
- **Edition inspected:** Sixth edition, 2019, ISBN `978-0-12-811905-1`
- **Raw inventory family:** `K0350`–`K0363`
- **Primary records:** `K0352` Chinese edition; `K0353` English edition
- **Supporting records:** solution manual, errata and online Appendices D–M
- **Review level:** L1
- **Priority after review:** S

#### Verified scope

The main book provides seven major architectural lenses:

1. quantitative design and analysis, including performance, power, cost, dependability and summary metrics;
2. memory hierarchy design;
3. instruction-level parallelism, branch prediction, dynamic scheduling, speculation, multiple issue and SMT;
4. vector, SIMD and GPU data-level parallelism;
5. multicore, directory coherence, synchronization and memory consistency;
6. warehouse-scale computers and cloud computing;
7. domain-specific architectures.

The included appendices cover RISC-V ISA principles, cache basics and pipelining; the online appendices extend into storage, interconnection networks, vector processors, VLIW/EPIC, large multiprocessors, arithmetic, ISA surveys, advanced address translation and historical references.

Its central contribution to ArchLab is not a single microarchitecture. It is the engineering discipline of defining the research question, selecting representative workloads and metrics, measuring real systems, using simulation, reporting assumptions, and reasoning quantitatively about cost-performance-energy-availability trade-offs.

#### ArchLab value

This is the project’s **primary architecture and evaluation framework**:

- Chapter 1 anchors statistics, metric definitions, Amdahl-style reasoning and experiment reporting.
- Appendix A provides a RISC-V-oriented ISA review for the functional core.
- Appendix C and Chapter 3 frame the timing and OOO core.
- Chapter 2 and Appendix B support cache/memory work.
- Chapter 5 supports coherence, consistency and multicore.
- Appendix F supports interconnect work.
- Chapter 6 supports datacenter workloads and the project’s top-down system view.
- Chapter 7 supports future accelerator modeling.

The Chinese and English editions are reading alternatives. The solution manual is a checking aid, not an independent primary source. Errata and companion appendices remain attached to the same canonical family.

#### Do not copy literally

- This is not a software architecture manual for building a simulator framework.
- Algorithm diagrams do not define ownership, lifetime, serialization or C++ APIs.
- Real-machine examples and numerical parameters are historical snapshots.
- A simulated IPC number is meaningful only after workload selection, warmup, sampling, model scope and validation are documented.

#### Targeted reading plan

The user’s separate morning reading plan remains unchanged. For simulator construction, consult selectively:

- **Now and throughout:** Chapter 1.
- **Functional RV64/Linux:** Appendix A and relevant Appendix L material.
- **Timing/OOO:** Appendix C and Chapter 3.
- **Memory/coherence/NoC:** Chapter 2, Appendix B, Chapter 5 and Appendix F.
- **Heterogeneous/datacenter:** Chapters 4, 6 and 7.

#### Source locators

- English: `计算机体系结构量化方法第六版英文版.pdf`, Drive ID `1bbV-leXpE0tAiqQmkkQHsA8t0YDA8PKh`
- Chinese: `计算机体系结构量化研究方法第6版 ... .pdf`, Drive ID `1PQrrRk1VFxreIQC5SmXtVoFThL9V8t8a`
- Solution manual: Drive ID `1Enb8rMuspIImCPTx2NQWV2zjwGf6wtl0`

---

### CPU-002 — Processor Microarchitecture: An Implementation Perspective

- **Authors:** Antonio González, Fernando Latorre and Grigorios Magklis
- **Edition inspected:** Morgan & Claypool Synthesis Lecture #12, 2011
- **ISBN:** `9781608454525` paperback; `9781608454532` ebook
- **Raw inventory records:** `K0382` `Processor Microarchitecture.pdf`; `K0389` `Processor_Microarchitecture.pdf`
- **Canonical relation:** filename variants mapped to the same 2011 work; byte identity still requires hash verification
- **Review level:** L1
- **Priority after review:** S

#### Verified scope

This compact book follows the actual pipeline structures:

```text
Fetch
→ Decode
→ Rename / Allocation
→ Issue
→ Execute / Write-back
→ Commit
```

It then studies the implementation details that a conceptual architecture text usually compresses:

- cache tag/data-array organization, serial versus parallel access and multiporting;
- lockup-free caches and implicit, explicit and in-cache MSHR organizations;
- I-cache, BTB, return-address stack and branch predictors;
- RISC decoding, x86 length decoding and dynamic translation to internal operations;
- three register-renaming organizations: ROB-based, rename-buffer and merged physical register file;
- read-before-issue versus read-after-issue;
- issue-queue allocation, wakeup, select, reclamation and distributed organizations;
- non-speculative and speculative memory disambiguation;
- execution units, AGUs, bypass networks and clustering;
- architectural state management and recovery from branch misprediction or exception.

The implementation perspective gives several important resource-lifecycle rules. Allocation reserves ROB, issue-queue and load/store-queue resources before later execution. The ROB retains enough information to retire or squash an instruction. The commit stage is the boundary that makes out-of-order execution appear sequential. Speculative state must therefore be recoverable independently from committed architectural state.

#### ArchLab value

This is the project’s **primary compact implementation reference for the future detailed core**. It is especially valuable for turning conceptual blocks into explicit state, queues, tags, tables, ports and lifecycle transitions.

Direct uses:

- define the responsibilities and state of Fetch, Decode, Rename, Issue, Execute and Commit components;
- design `current`/`next` state for cycle-accurate structures;
- define resource reservation and release rules;
- design precise exception and misprediction recovery;
- connect load/store execution to transactions, MSHRs and the memory hierarchy;
- make bypass, register-file ports, cache banks and issue selection visible timing resources.

#### Do not copy literally

- The examples emphasize contemporary processors as of 2011, especially x86 and several historical commercial cores.
- The first ArchLab core should not implement all rename alternatives, clustered bypass networks or speculative memory disambiguation simultaneously.
- MSHR and LSQ entries are microarchitectural state; they must not leak into the generic transaction protocol.
- Detailed hardware organizations should be selected as separate model implementations rather than accumulated as configuration booleans in one class.

#### Targeted reading plan

- **Before cache timing:** Chapter 2.
- **Before timing frontend:** Chapters 3 and 4.
- **Before OOO rename/issue:** Chapters 5 and 6.
- **Before detailed execution:** Chapter 7.
- **Before precise recovery:** Chapter 8.

---

### SOC-001 — Modern System-on-Chip Design on Arm

- **Author:** David J. Greaves
- **Edition inspected:** Arm Education, 2021, ePDF ISBN `978-1-911531-37-1`
- **Raw inventory records:** `K0025` and `K0345`, both named `ModernSoC_textbook.pdf`
- **Canonical relation:** same filename and observed size (`16,018,114` bytes); probable duplicate copies, byte hash still pending
- **Review level:** L1
- **Priority after review:** S
- **Classification correction:** primary domain changed from `CPU / ISA / Microarchitecture` to `SoC Integration / Interconnect / ESL`; CPU is a secondary domain

#### Verified scope

This book is much broader than its coarse filename classification suggested. It covers:

- functional models, architectural partition, hardware/software co-design, IP blocks and the SoC design flow;
- processors, MMU/IOMMU, multicore/SMT, caches/coherence, interrupts, memory technology and device blocks such as UART, timer, DMA and mailbox;
- transaction-oriented interconnect, ordering, tags, error responses, AXI, directory coherence, NoC routing, virtual channels, deadlock, credit flow control, CHI, SERDES, PCIe, CXL and NVLink;
- performance metrics, contention, queueing, QoS, power, debug, RAS and security;
- ESL abstraction levels, SystemC, TLM 1.0/2.0, loose and approximate timing, temporal decoupling, non-blocking transport and transactors;
- processor models at different levels of abstraction, DMI, power/performance models and virtual platforms;
- design-space exploration, subsystem partition, bandwidth/storage/operation requirements, accelerator design, formal methods and assertions.

The abstraction taxonomy is directly relevant to ArchLab. It distinguishes functional, memory-accurate, untimed/cycle-lumped TLM, stochastic/loosely timed TLM, approximately timed TLM, cycle-accurate, net-level and mixed-signal models. It also distinguishes programmer-visible state from hidden pipeline and hazard-management state.

The interconnect sections reinforce a request/response view: initiators such as CPUs and DMA engines send transactions to targets; read operations need responses, writes may be acknowledged, and tags, ordering, failures, translation and ECC status belong to the transaction/interconnect contract. The TLM section further shows that transactors are the correct place to bridge high- and low-level modeling styles.

#### ArchLab value

This source connects several project tracks that are otherwise easy to design independently:

- functional platform and real device semantics;
- typed transactions and request/response completion;
- queueing, contention and timing channels;
- CHI/NoC and PCIe/CXL boundaries;
- per-component fidelity and transactors;
- design-space exploration, statistics and resolved configuration;
- assertions and protocol invariants.

It is therefore a cross-cutting SoC framework source, not primarily an OOO-core book.

#### Do not copy literally

- ArchLab is RISC-V-centered and is not adopting Arm-specific APIs or AMBA as its universal internal interface.
- SystemC/TLM mechanisms are reference semantics and adapter targets, not a requirement to replace the native C++ simulation kernel.
- Approximate timing must not be presented as cycle accuracy.
- The broad book should be consulted by subsystem; reading it linearly must not interrupt the current M0 milestone.

#### Targeted reading plan

- **M0 transactions/channels:** Sections 3.1 and 5.1–5.5.
- **Functional Linux platform:** Chapters 1–2.
- **NoC/CHI:** Chapter 3.
- **Stats/config/design exploration:** Chapters 4 and 6.
- **Validation:** Chapter 7.

#### Source locators

- Drive copy 1: ID `1vbWDW61B5PLjFuYBomd8tfxQs6Qs5Qek`
- Drive copy 2: ID `1ilg5pZDyQrV5fR1kh8Y3CrRb_3h9t-he`

## 2. How the four sources divide responsibility

These books overlap, but they are not substitutes:

| Source | Primary role in ArchLab |
|---|---|
| Computer Architecture: A Quantitative Approach | What question to ask, what metric to use, and how to evaluate trade-offs |
| Modern Processor Design | Why superscalar structures exist and how instruction/register/memory flows interact |
| Processor Microarchitecture | How detailed pipeline structures, tables, queues, ports and recovery are organized |
| Modern System-on-Chip Design on Arm | How cores, memory, devices and interconnect are composed across modeling abstractions |

This division prevents a common mistake: using an implementation book as the experimental method, or using a quantitative textbook as if it defined C++ object lifetimes and simulator APIs.

## 3. ArchLab model boundaries derived from Batch 2

The following are **project conclusions informed by the sources**, not verbatim requirements from any single book.

### 3.1 Functional core

The first RV64 core should model software-visible behavior and architectural state:

```text
PC
GPR/FPR as enabled
CSR and privilege state
trap/interrupt-visible state
architectural memory effects
MMU/page-table behavior required by the selected privilege model
```

It should not pretend to model predictor tables, physical registers, ROB, issue queue, LSQ, bypass paths or cache-bank conflicts. A functional instruction completes at an architectural boundary and must support precise traps and differential checking.

### 3.2 Timing in-order core

The timing core adds explicit pipeline and resource timing without adding speculative OOO state:

```text
stage current/next state
stage queues or latches
hazards and stalls
functional-unit latency
memory request/completion timing
clock-domain behavior
```

It should reuse the architectural semantics and memory transaction contract rather than duplicate the ISA implementation.

### 3.3 Detailed OOO core

The OOO model introduces explicit speculative structures:

```text
Fetch / Predictor
Decode
Rename Map / Free List
ROB
Issue Queue / Wakeup / Select
Execution Units / Bypass
LSQ / Memory Disambiguation / Replay
Commit
Recovery
```

Architectural state changes become final only at commit. Speculative state must be squashable or restorable. Resource allocation and release must have explicit ownership and invariants.

### 3.4 SMT and vector

SMT is a resource-sharing policy over a working OOO core, not the starting point. Vector/SIMD execution similarly requires its own architectural and microarchitectural contracts rather than being represented merely by a wider scalar instruction.

### 3.5 SoC boundary

The core should communicate with the memory/platform through stable requests and completions. MSHR, ROB and LSQ identifiers may be carried as adapter metadata, but their internal entries must not become the public interconnect protocol.

## 4. Validation consequences

Batch 2 strengthens four separate validation layers:

1. **ISA correctness:** differential testing against Spike/NEMU and precise trap tests.
2. **Pipeline invariants:** every instruction and resource has one owner; no double allocation/release; current/next transitions are deterministic.
3. **Speculation correctness:** wrong-path work cannot modify committed state; recovery restores rename, queue and control state.
4. **Performance validation:** documented workloads, warmup, ROI, sampling, assumptions and calibration; functional correctness does not validate timing accuracy.

## 5. Effect on the current milestone

This batch does **not** change the development order. M0.3 Transaction/Completion remains active. The CPU findings explain why the generic transaction layer must remain independent of future ROB, LSQ and MSHR implementations, and why completion identity and exact-once lifecycle rules must be correct before the detailed core depends on them.
