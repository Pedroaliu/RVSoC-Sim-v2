# Detailed Review Batch 4 — Performance, Datacenter and Evaluation Methodology

> Review level: **L1**  
> Date: 2026-08-03

L1 means the actual table of contents, abstract/preface, introduction and selected methodology sections were inspected. It does not mean cover-to-cover reading. Statistical derivations, queueing proofs and concrete experiment templates will be promoted to L2/L3 when they are used by ArchLab experiments.

## 1. Canonical works reviewed

### PERF-001 — The Art of Computer Systems Performance Analysis

- **Author:** Raj K. Jain
- **Edition inspected:** 1991, Wiley
- **Raw records:** two same-title, same-size copies in Drive
- **Review level:** L1
- **Priority after review:** S

#### Verified scope

The book covers the complete performance-evaluation workflow rather than a single tool or formula:

- common evaluation mistakes and a systematic approach;
- selecting evaluation techniques and performance metrics;
- workload types, selection, representativeness and characterization;
- software, hardware, firmware and distributed-system monitors;
- capacity planning and benchmarking pitfalls;
- data presentation and ratio/percentage games;
- probability, confidence intervals, regression and sample-size selection;
- factorial experimental design and analysis of variance;
- simulation types, event-set algorithms, verification, validation, transient removal, stopping criteria and variance reduction;
- queueing theory, Little's Law, operational laws and queueing networks.

The important lesson is methodological: performance work can be wrong even when every measured number is accurate. The wrong question, workload, metric, baseline, ratio, averaging method or graph can still yield a misleading conclusion.

#### ArchLab value

This is the broad methodology backbone for simulator validation and for the user's real-server performance work.

Direct uses:

- require every experiment to state the question, system, factors, levels, metrics and workload;
- distinguish measurement, analytical modeling and simulation;
- define repeated-run and confidence-interval rules;
- prevent benchmark and ratio manipulation;
- validate simulator models with deterministic cases, simplified cases, traces, continuity checks and real-system measurements;
- structure Top-down reports so raw counters, derived metrics and conclusions remain distinguishable.

#### Do not copy literally

- Many benchmark names and monitor examples are historically dated.
- The statistical and experimental principles remain useful, but modern workloads and tools must replace old examples.
- Not every deterministic architecture test needs stochastic confidence intervals; the method must match the experiment.

#### Targeted reading

- **Now:** Chapters 2, 3, 4, 5, 10, 11, 12, 13, 16, 24 and 25.
- **Before automated parameter studies:** Chapters 17–23.
- **Before analytical queue models:** Chapters 30–33.

#### Source locators

- Primary Drive ID: `1v17Ivc66NO-zDwpYwFQfIODTkmhu0eNi`
- Probable duplicate ID: `1w5AstwtB3Y2QNZJ62Np-ESEEaBc8uosy`

---

### PERF-002 — Performance Modeling and Design of Computer Systems: Queueing Theory in Action

- **Author:** Mor Harchol-Balter
- **Edition inspected:** 2013, Cambridge University Press
- **Raw records:** full-title copy plus a truncated-title `Performance Modeling and.pdf` candidate
- **Review level:** L1
- **Priority after review:** S

#### Verified scope

This book develops analytical performance models for systems questions that matter to computer architects and systems designers:

- open and closed queueing networks;
- throughput, utilization and response time;
- Little's Law, Forced Flow and other operational laws;
- Markov chains, Poisson processes and M/M/1;
- server farms, M/M/k and capacity provisioning;
- square-root staffing;
- queueing networks, routing and product-form solutions;
- scheduling, variability, heavy-tailed service demands and server organization;
- energy-delay and resource-design tradeoffs.

Its framing is useful because it emphasizes counterintuitive design questions: one fast server versus many slow servers, balanced versus deliberately unbalanced load, individual greedy routing versus system-wide performance, and the impact of service-time variability.

#### ArchLab value

This is the analytical counterpart to the discrete-event simulator. It helps decide when a simulator is necessary and when a compact model already explains the system.

Direct uses:

- validate queue occupancy, outstanding requests and latency using Little's Law;
- reason about MSHR count, memory-controller queues, NoC buffers and PCIe outstanding tags;
- estimate saturation points before detailed simulation;
- build synthetic traffic generators with explicit arrival and service assumptions;
- separate utilization from response time and expose the nonlinear knee near saturation;
- size replicated servers/ports/channels and compare pooled versus partitioned resources.

#### Do not copy literally

- Markovian assumptions are modeling assumptions, not facts about real workloads.
- An M/M/1 or M/M/k result must not be presented as a detailed CPU/NoC/DRAM prediction without validating arrivals, service times and independence assumptions.
- Queueing theory complements detailed simulation; it does not replace protocol state, ordering or contention mechanisms.

#### Targeted reading

- **Now:** Chapters 1, 2, 6 and 7.
- **Before queue/backpressure experiments:** Chapters 13–18.
- **Before datacenter/server-farm studies:** Chapters 14, 15 and later scheduling/variability chapters.

#### Source locators

- Primary Drive ID: `1VIezrZzKxeKelftOhJ97A3ck4dPGvfON`
- Candidate related/duplicate record: `1hyIVZ9FYYefkzXdfxv2VKYOouBwBRKHW`

---

### PERF-003 — The Datacenter as a Computer: Designing Warehouse-Scale Machines

- **Authors:** Luiz André Barroso, Urs Hölzle and Parthasarathy Ranganathan
- **Primary edition inspected:** Third Edition, 2018/2019
- **Related edition inspected:** Second Edition, 2013, with Jimmy Clidaras
- **Review level:** L1
- **Priority after review:** S

#### Canonicalization

The two Drive files are not duplicates:

- `The Datacenter as a Computer.pdf` is the **Second Edition** (2013);
- `The Datacenter as a Computer(1).pdf` is the **Third Edition** (2018/2019).

The third edition is the primary source because it adds modern workloads such as video and machine learning, specialized accelerators, updated storage/network building blocks, power/cooling and uptime material. The second edition remains a related historical edition.

#### Verified scope

The book's central claim is that a warehouse-scale datacenter is not merely a collection of servers. Hardware, software, storage, networking, power, cooling, monitoring and fault handling form one computer at warehouse scale.

The inspected contents cover:

- WSC architecture and cost efficiency;
- server, storage and network building blocks;
- workload diversity and cluster/application software;
- service-level, performance-debugging and health-monitoring infrastructure;
- power usage, cooling and facility design;
- failures, repairs, availability and total-cost tradeoffs;
- tail behavior and the need to reason beyond averages;
- accelerator and machine-learning workloads in the third edition.

#### ArchLab value

This book defines the outermost level of the planned ArchLab Top-down view. A single CPU or SoC metric is only useful when tied to service behavior and system constraints.

Direct uses:

- model synthetic cloud-service traffic and multi-node systems;
- preserve workload → software → OS/VM → device → SoC causal paths;
- support tail latency, availability, power and cost metrics alongside throughput;
- justify future SimBricks/network/distributed simulation integration;
- frame strong-node versus scale-out tradeoffs without assuming that either is universally superior;
- guide the visualization hierarchy from service SLO down to cycle-level bottlenecks.

#### Do not copy literally

- The book is a datacenter architecture source, not a detailed CPU microarchitecture model.
- Historical Google hardware details are examples, not fixed modern baselines.
- Warehouse-scale performance does not imply that single-node efficiency is irrelevant; the correct level depends on where the service bottleneck and cost lie.

#### Targeted reading

- **Now:** Chapters 1–3 and monitoring/tail-tolerance sections.
- **Before power/RAS:** power, cooling, failures and repair chapters.
- **Before distributed workloads:** storage, network and software-infrastructure chapters.

#### Source locators

- Third Edition Drive ID: `1d7ZWM6IJGnYDu6Y2bxvzrHROArSnB4fl`
- Second Edition Drive ID: `1FXoLfBNGRbD5LPnX14_nG34rnhaEA83R`

---

### PERF-004 — Computer Architecture Performance Evaluation Methods

- **Author:** Lieven Eeckhout
- **Edition inspected:** 2010, Morgan & Claypool
- **Review level:** L1
- **Priority after review:** S

#### Verified scope

This book is narrower and more architecture-specific than Raj Jain's work. It covers:

- performance metrics for single-thread, multithread and multiprogram workloads;
- throughput, normalized turnaround and averaging choices;
- representative workload selection using PCA, clustering and experimental designs;
- empirical, mechanistic and hybrid analytical modeling;
- interval modeling of frontend and backend miss events;
- functional, full-system, trace-driven and execution-driven simulation;
- sampled simulation, statistical simulation, parallel simulation and FPGA acceleration.

Its main warning is that every link in the chain matters: workload, metric, modeling approach, simulator execution and result interpretation. Weakness in any one can drive architectural research in the wrong direction.

#### ArchLab value

This is the primary source for defining ArchLab's simulation-mode taxonomy and experiment metadata.

Direct uses:

- define when functional, trace-driven, execution-driven and full-system modes are appropriate;
- build workload-reduction and ROI selection methods without silently changing representativeness;
- choose metrics for single-core, SMT, multiprogram and throughput experiments;
- distinguish mechanistic models from empirical fitted models;
- design simulation acceleration while retaining error estimates;
- record which effects are excluded by a chosen mode.

#### Do not copy literally

- Workload-reduction methods can hide rare or tail behavior if the feature space excludes it.
- Sampled simulation requires warmup and state-reconstruction discipline.
- A faster simulation technique is not automatically acceptable; accuracy must be reported relative to the research question.

#### Targeted reading

- **Now:** Chapters 2, 3 and 5.
- **Before Top-down analytical models:** Chapter 4.
- **Before ROI/sampling acceleration:** sampled/statistical/parallel/hardware-accelerated simulation chapters.

#### Source locator

- Drive ID: `1uJvEJK0K9u2xoyljxk9DiTLpLliA5RDp`

## 2. Batch-level conclusions

The four sources converge on a single evaluation discipline:

```text
Research question
    ↓
System boundary and assumptions
    ↓
Workload and representativeness
    ↓
Factors and levels
    ↓
Metric and aggregation rule
    ↓
Evaluation method and fidelity
    ↓
Warmup / ROI / repetitions / seeds
    ↓
Verification and validation
    ↓
Uncertainty and applicability range
    ↓
Conclusion
```

A result is not trustworthy merely because a simulator produced many decimal places.

## 3. Immediate ArchLab implications

### 3.1 Every run needs a resolved experiment manifest

At minimum:

```text
research_question
workload
system_config
component_fidelity
warmup
roi
seed
repetitions
metrics
reference_or_validation_target
```

### 3.2 Statistics need provenance

The simulator should distinguish:

- raw events/counters;
- derived metrics;
- normalization baseline;
- aggregation method;
- confidence/variance information;
- excluded or unsupported effects.

### 3.3 Top-down must preserve causal attribution

The planned visualization should not merely show hot components. It should connect:

```text
Service objective
→ workload phase
→ VM/process/thread
→ software stall
→ architectural bottleneck
→ queue/resource
→ transaction/event/cycle
```

### 3.4 Analytical models are validation partners

Little's Law and queueing bounds should be used to sanity-check simulator results. Detailed simulation is required when state-dependent scheduling, ordering, contention feedback or protocol behavior dominates.

### 3.5 Accuracy is question-relative

A mode is valid only for a declared question. Functional execution can validate software-visible behavior; fixed-latency timing can test lifecycle logic; detailed DRAM/NoC models are needed for contention and timing-feedback claims; datacenter conclusions may require tail, failure and multi-node behavior.
