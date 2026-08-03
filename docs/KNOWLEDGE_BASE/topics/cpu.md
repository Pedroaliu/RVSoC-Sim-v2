# CPU / ISA / Microarchitecture Topic Map

> Last updated: 2026-08-03  
> Current maturity: source map and model boundaries; no detailed OOO implementation contract yet

This document connects reviewed CPU sources to ArchLab milestones. It deliberately separates source-supported observations from project design conclusions.

## 1. Primary sources

| Canonical ID | Source | Role |
|---|---|---|
| ARCH-001 | Computer Architecture: A Quantitative Approach, 6th ed. | quantitative method, ISA/ILP/memory/coherence/datacenter context |
| CPU-001 | Modern Processor Design | processor-performance reasoning and superscalar flow organization |
| CPU-002 | Processor Microarchitecture: An Implementation Perspective | concrete pipeline structures and implementation trade-offs |
| SOC-001 | Modern System-on-Chip Design on Arm | SoC composition, interconnect and multi-abstraction modeling |

Detailed notes: `../reviews/batch-02-cpu-isa-ooo.md`.

## 2. Stable conceptual split

### 2.1 Architecture

Architecture is the software-visible contract. For the first ArchLab RISC-V platform this eventually includes:

- instruction semantics and PC progression;
- integer and optional floating/vector registers;
- privilege and CSR behavior;
- traps, interrupts and exception ordering;
- architecturally visible memory ordering and fences;
- virtual-memory behavior required by the chosen privilege profile;
- device and interrupt semantics visible to firmware and Linux.

The architecture contract should not expose branch-predictor entries, physical registers, ROB slots, issue entries, cache banks or NoC virtual channels.

### 2.2 Microarchitecture

Microarchitecture is one implementation of the architectural contract. It determines when and with which resources work occurs:

- pipeline depth and width;
- predictor organization;
- rename and physical-register scheme;
- issue policy and execution resources;
- bypass topology;
- LSQ and memory-disambiguation policy;
- cache, MSHR and TLB timing;
- commit and recovery mechanism;
- SMT resource-sharing policy.

Different microarchitectures should be replaceable without changing the architectural reference behavior.

### 2.3 Simulator framework

The simulator framework provides capabilities shared by all models:

- deterministic time and event order;
- object lifecycle;
- typed ports and links;
- transaction and completion identity;
- queueing, backpressure and retry;
- current/next state;
- configuration, statistics and trace;
- checkpoint/restore and fault hooks.

CPU structures should use these contracts rather than inventing private event, lifetime or request semantics.

## 3. Planned model progression

```text
Architectural semantics / reference execution
    ↓
Functional RV64 core
    ↓
Timing in-order core
    ↓
Detailed OOO core
    ↓
SMT
    ↓
RISC-V Vector / heterogeneous interaction
```

### 3.1 Functional RV64 core

Purpose:

- run OpenSBI and Linux correctly;
- provide a self-written architectural reference;
- support differential testing;
- establish traps, privilege, MMU and device-visible behavior.

Characteristics:

- sequential instruction semantics;
- architectural state only;
- no performance claims from predictor, pipeline, ROB, LSQ or cache details;
- one well-defined instruction/trap boundary;
- memory accesses expressed through the platform transaction contract.

### 3.2 Timing in-order core

Purpose:

- introduce pipeline timing, hazards and resource stalls without OOO speculation;
- validate current/next state and cycle modeling;
- expose frontend, execution and memory-stall statistics.

Expected structures:

- stage latches or queues;
- hazard/scoreboard logic;
- execution-unit latency and availability;
- request/completion waiting state;
- flush/reset behavior;
- clock-domain integration.

The exact stage count is a model parameter or implementation choice, not part of the ISA.

### 3.3 Detailed OOO core

Purpose:

- model ILP limits, speculation, resource contention and recovery;
- support research on predictor, rename, scheduling, LSQ, cache and SMT policies.

Expected component responsibilities:

| Component | Responsibility |
|---|---|
| Fetch / Predictor | choose speculative instruction stream and track prediction metadata |
| Decode | convert instruction encoding into stable semantic operations and resource requirements |
| Rename / Allocation | remove name dependences and reserve ROB/IQ/LSQ/physical-register resources |
| Issue | track operand readiness and select ready operations under resource limits |
| Execute | model functional units, address generation and result timing |
| Write-back / Wakeup | publish results to dependents and relevant structures |
| LSQ | enforce memory ordering, forwarding, disambiguation and replay policy |
| ROB / Commit | maintain program-order retirement and precise architectural state |
| Recovery | squash or restore speculative state after misprediction, exception or replay |

This table is a responsibility map, not a promise that each item becomes exactly one C++ class.

## 4. State and ownership invariants to carry into later design

These are ArchLab design targets derived from the reviewed sources and existing framework principles:

1. **Architectural state is committed in program order.** Wrong-path or faulting work cannot become permanent.
2. **Speculative state has an explicit owner and recovery path.** Rename mappings, physical registers, queue entries and predictor checkpoints cannot be restored by ad hoc cleanup.
3. **Resource allocation precedes use and has one release event.** ROB/IQ/LSQ/free-list entries must not be double allocated or double freed.
4. **Memory requests have stable identity.** LSQ/MSHR bookkeeping maps to generic transaction keys without exposing internal pointers as protocol identity.
5. **A completion is not equivalent to commit.** A load may receive data before the instruction can retire; a store may commit before all external effects are globally visible, depending on the model.
6. **Functional and timing correctness are separate.** Matching final register values does not prove predictor, cache, queue or cycle accuracy.
7. **Current/next separation applies to cycle-visible structures.** Same-cycle updates need deterministic rules instead of relying on C++ call order.

## 5. Statistics hierarchy

The three-flow organization from Modern Processor Design suggests a stable top-level view:

```text
Core throughput
├── Instruction-flow loss
│   ├── I-cache / ITLB
│   ├── branch target and direction prediction
│   ├── redirect / recovery
│   └── decode bandwidth
├── Register-data-flow loss
│   ├── rename / free-list pressure
│   ├── issue-queue pressure
│   ├── operand readiness
│   ├── execution-unit availability
│   └── bypass / register-file constraints
└── Memory-data-flow loss
    ├── LSQ pressure
    ├── address readiness
    ├── ordering / forwarding / replay
    ├── cache / MSHR pressure
    └── memory-system latency and bandwidth
```

This is compatible with the project’s future Top-down visualization: the user should be able to start from lost throughput and descend into the exact queue, dependency or transaction that caused it.

## 6. Configuration principle

Widths and capacities can be data:

```text
fetch width
rename width
commit width
ROB entries
IQ entries
LSQ entries
physical registers
functional-unit counts and latencies
```

Fundamentally different organizations should remain separate implementations or policies:

- ROB-based versus merged-register-file renaming;
- centralized versus distributed issue;
- blocking versus nonblocking cache;
- conservative versus speculative memory disambiguation;
- in-order versus OOO core.

Avoid one giant core class controlled by dozens of booleans.

## 7. Validation plan by layer

### Functional core

- instruction and CSR unit tests;
- trap/interrupt and privilege tests;
- differential execution against Spike/NEMU;
- Linux boot milestones;
- deterministic replay of failures.

### Timing in-order

- stage-transition tests;
- hazard/stall/flush tests;
- current/next invariants;
- fixed synthetic instruction streams with hand-computed cycles;
- memory completion at selected ticks.

### OOO

- rename and free-list conservation;
- ROB order and exact-once retirement;
- issue readiness and selection;
- load/store ordering and forwarding;
- branch recovery and precise exception tests;
- wrong-path side-effect suppression;
- cross-check final architectural state against the functional core;
- performance calibration only after correctness tests pass.

## 8. Explicitly deferred decisions

Batch 2 does not choose:

- a specific predictor;
- exact pipeline depth or width;
- a rename organization;
- centralized versus distributed issue;
- store-commit/drain details;
- speculative memory disambiguation policy;
- SMT partitioning policy;
- Vector microarchitecture.

Those decisions belong to the timing/OOO milestones and will use L2/L3 reviews plus executable experiments.
