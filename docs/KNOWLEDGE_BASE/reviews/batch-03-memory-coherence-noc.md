# Detailed Review Batch 3 — Memory, Cache, Coherence and NoC

> Review level: **L1**  
> Date: 2026-08-03

L1 means that the actual table of contents, abstract, introduction and selected architecture sections were inspected. It does not mean cover-to-cover reading. Detailed protocol state machines, DRAM command timing and router algorithms will be promoted to L2/L3 when the corresponding ArchLab milestone begins.

## 1. Canonical works reviewed

### MEM-002 — Memory Systems: Cache, DRAM, Disk

- **Authors:** Bruce Jacob, Spencer W. Ng and David T. Wang
- **Edition inspected:** 2008
- **Raw inventory record:** `K0114`
- **Review level:** L1
- **Priority after review:** S

#### Verified scope

This book takes a whole-hierarchy view rather than treating cache, DRAM and storage as unrelated chapters. Its overview explicitly covers:

- locality and the reason a hierarchy exists;
- latency, bandwidth, cost and capacity as competing figures of merit;
- systemic behavior that appears only when components interact;
- cost/performance, power, reliability and virtual-memory concerns that cross levels;
- cache logical organization, content management, consistency management and implementation;
- DRAM internals, device organization, interface/protocol evolution and memory-system organization;
- disk and storage-system behavior.

The cache section distinguishes logical organization from policy and implementation. It covers tags, blocks and sets; transparent and software-managed storage; inclusion/exclusion; partitioning, prefetching and locality management; consistency responsibilities; and physical implementation details.

The DRAM section begins at cell/array and device organization, then moves upward toward commands, timing, interfaces and system organization. This is valuable because a controller cannot be modeled accurately by attaching one constant latency to a cache miss: bank state, command legality, buses, refresh and request interactions matter.

A recurring theme is that local component optimization does not necessarily improve the whole system. The book uses holistic examples to show that design decisions must be evaluated across the hierarchy.

#### ArchLab value

This is the broad primary reference for the future memory subsystem:

```text
Core / DMA requester
        ↓
L1 / L2 / LLC
        ↓
coherence and directory
        ↓
NoC / memory-side interconnect
        ↓
controller queues and scheduler
        ↓
DRAM channel / rank / bank / row
```

It supports the project principle that function, policy, resource occupancy and physical timing must be represented separately enough to permit model replacement.

#### ArchLab design implications

The following are project inferences based on the book’s hierarchy and cross-cutting treatment:

1. A cache model should not be one monolithic `access()` function. At minimum, distinguish:
   - address lookup and hit/miss result;
   - cache-line state and data update;
   - miss allocation and merge;
   - eviction/writeback responsibility;
   - timing resources such as ports, banks, queues and fill paths.
2. Cache functional correctness and cache timing fidelity must be independently replaceable.
3. A DRAM timing backend should receive explicit requests and return explicit completions; its bank/rank/channel state remains private to the backend.
4. The public transaction identity must survive through the hierarchy, while cache MSHR, directory and DRAM queue entries remain subsystem-private implementation state.
5. Statistics must support end-to-end attribution: queueing at cache, NoC, controller, command scheduler and device timing must not collapse into one generic “memory latency.”

#### Do not copy literally

- Technology examples and exact device parameters are from the 2008 design space.
- Disk chapters are useful architectural context but are not part of the first Linux/UART functional milestone.
- Circuit-level cache and DRAM details should not be forced into every simulation mode.
- The book’s breadth does not mean all hierarchy levels should be implemented simultaneously.

#### Targeted reading plan

- **Before cache model:** overview and Chapters 1–4.
- **Before detailed DRAM backend:** DRAM overview, device organization, commands/timing and controller/system chapters.
- **Before RAS:** reliability sections and DRAM failure-sensitive organization.
- **Before storage/NVMe:** disk and storage sections.

#### Source locator

- File: `Memory systems cache, DRAM, disk by Bruce Jacob, Spencer Ng, David Wang (z-lib.org).pdf`
- Drive ID: `1YYS7rbXdAeMFp-jRHjAD0xIKBfpz_nha`

---

### MEM-003 — The Memory System: You Can’t Avoid It, You Can’t Ignore It, You Can’t Fake It

- **Author:** Bruce Jacob, with contributions by Sadagopan Srinivasan and David T. Wang
- **Edition inspected:** 2009
- **Raw inventory record:** `K0384`
- **Canonical relation:** not a duplicate of *Memory Systems: Cache, DRAM, Disk*; this is a focused synthesis lecture on why memory detail and model fidelity matter
- **Review level:** L1
- **Priority after review:** S

#### Verified scope

The lecture covers:

1. how program data and physical storage interact;
2. memory-system organization and DRAM basics;
3. the difference among fixed-latency, simple queueing and cycle-accurate memory models;
4. how inaccurate models distort conclusions as core count, bandwidth pressure and prefetch activity increase;
5. future memory-system trends and virtual-memory implications.

Its most important result for a simulator project is methodological. A fixed-latency model ignores bandwidth constraints. A simple queueing model adds first-order bandwidth effects but still ignores request reordering, coalescing and DRAM device state. The source shows that such simplifications may appear acceptable in a low-load region but diverge badly under contention. It also shows that model error grows as systems become more complex and prefetching increases traffic.

The lecture explicitly argues that a real memory system produces a latency distribution, not one universal latency constant.

#### ArchLab value

This is the strongest source in the library for defining **memory fidelity contracts**. It prevents us from accidentally presenting a functional or first-order timing backend as a detailed DRAM performance model.

#### ArchLab design implications

ArchLab should eventually expose at least these distinct memory backends:

```text
FunctionalMemory
    correct bytes and access semantics
    no performance claim

FixedLatencyMemory
    deterministic completion delay
    useful for bring-up and isolation
    no saturation or bank-state claim

QueuedMemory
    capacity, service rate, queueing and backpressure
    first-order contention model
    no detailed DRAM-command claim

DetailedDRAMBackend
    address mapping, controller queues, scheduling,
    channel/rank/bank/row state, commands, timing,
    refresh and bus turnaround
```

Every experiment must record which backend was used. A result obtained with `FixedLatencyMemory` must not be described as a DRAM timing result.

#### Do not copy literally

- DDR-800-era numerical examples and the exact studied systems are historical evidence, not current parameters.
- The source’s criticism of simple models does not imply they are useless; they are legitimate when the research question and limitations are declared.
- Detailed DRAM timing should not block the first functional Linux milestone.

#### Targeted reading plan

- **Read now:** Chapter 2, especially model comparison.
- **Before detailed memory:** Chapter 1 memory-system organization.
- **Before performance reports:** the discussion of latency distributions and complexity-driven model error.

#### Source locator

- File: `The Memory System.pdf`
- Drive ID: `1Smr0blGTh1iN1oxSgyLqXhwR5a-za0Ln`

---

### COH-001 — A Primer on Memory Consistency and Cache Coherence, Second Edition

- **Authors:** Vijay Nagarajan, Daniel J. Sorin, Mark D. Hill and David A. Wood
- **Edition inspected:** Second edition, 2020
- **Raw inventory records:** `K0413`, `K0474` second-edition copies; `K0416` first edition
- **Canonical relation:** `K0413` and `K0474` have the same title and observed size and are probable duplicate copies; `K0416` is the 2011 first edition and must remain a related edition, not a duplicate deletion
- **Review level:** L1
- **Priority after review:** S

#### Verified scope

The book clearly separates two concepts:

- **Memory consistency** defines the architecturally visible shared-memory behaviors that software is allowed to observe.
- **Cache coherence** keeps cached copies sufficiently coordinated to support the chosen model, but coherence by itself does not define the complete memory model.

The source states that the processor pipeline and coherence protocol jointly enforce the consistency model. A correct coherence protocol cannot compensate for a pipeline that exposes memory operations in an architecturally illegal order.

The book covers:

- coherence basics and invariants;
- sequential consistency;
- total store order and x86;
- weaker consistency models, including a RISC-V case study in the second edition;
- snooping protocols;
- directory protocols and their scalability motivation;
- protocol specification and deadlock concerns;
- heterogeneous CPU/GPU/accelerator consistency and coherence;
- formal specification and validation tools.

The directory chapter contrasts broadcast snooping with directory indirection. Directory protocols avoid requiring every cache controller to observe every request and avoid relying on one totally ordered broadcast fabric.

The second edition adds substantial material for non-CPU accelerators and for formal reasoning. It notes that heterogeneous devices may implement different consistency models and coherence mechanisms, making cross-device shared memory a semantic problem, not merely an interconnect problem.

#### ArchLab value

This is the primary semantic source for the planned multicore, CHI and CPU–accelerator memory system.

It establishes the ordering hierarchy:

```text
ISA memory model
        ↓
core pipeline ordering and fences
        ↓
cache/coherence protocol
        ↓
protocol messages and home/directory state
        ↓
NoC transport
```

No layer may silently redefine the semantics of the layer above it.

#### ArchLab design implications

1. The memory consistency model belongs to the architectural contract of the core and platform, not inside the NoC.
2. Coherence messages must be represented as protocol-level messages with explicit opcode, line identity, source/destination role and transaction context.
3. Stable cache states are insufficient for a realistic protocol model; transient states and outstanding protocol actions are required.
4. Directory/Home ownership state is protocol state. Router buffers and VC allocation are transport state. These must remain separate.
5. A fence, acquire/release operation or atomic operation is not “implemented by adding delay.” Its legal ordering effects must be represented.
6. The future validation stack must include:
   - ISA memory-model litmus tests;
   - protocol invariant checking;
   - message-race and transient-state tests;
   - deadlock/livelock tests;
   - heterogeneous shared-memory tests when accelerators are added.

#### Do not copy literally

- Do not begin with a full CHI-scale protocol. Start with a small protocol whose invariants and transient states can be tested completely.
- Do not conflate sequential consistency with a global fixed-latency memory.
- Do not infer that coherence alone guarantees consistency.
- First-edition and second-edition files are related sources, not interchangeable duplicates when citing accelerator/formal material.

#### Targeted reading plan

- **Before multicore cache:** Chapters 1–3.
- **Before directory model:** Chapters 6–8, with emphasis on state machines, transient states and deadlock.
- **Before RISC-V multicore:** the weak-model/RISC-V material.
- **Before CPU–GPU shared memory:** Chapter 10.
- **Before protocol verification:** Chapter 11.

#### Source locators

- Second edition: `A Primer on Memory Consistency and Cache Coherence 2nd.pdf`
- Second-edition Drive IDs: `1vgpJ7JtTzsqOEQzGAUfSjOrcZfOdV5uV`, `1k6Pvzip4J0-A_7jlTwdkSj6bq3iy08Jh`
- First edition: `A_Primer_on_Memory_Consistency_and_Coherence.pdf`
- First-edition Drive ID: `14p98Cs0-cGg67HFkm3gg9eCcl7PgSQtz`

---

### NOC-001 — On-Chip Networks, Second Edition

- **Authors:** Natalie Enright Jerger, Tushar Krishna and Li-Shiuan Peh
- **Edition inspected:** 2017
- **Raw inventory record:** `K0405`
- **Review level:** L1
- **Priority after review:** S

#### Verified scope

The book connects network design to the surrounding system architecture before introducing isolated router mechanisms. Its contents include:

- impact of coherence protocol and cache organization on network traffic;
- protocol requirements placed on the NoC;
- protocol-level network deadlock;
- Home-node, memory-controller, miss-status and transaction-status interactions;
- topology and topology metrics;
- routing algorithms;
- message, packet, flit and phit levels;
- store-and-forward, virtual cut-through and wormhole flow control;
- virtual channels;
- deadlock avoidance, escape VCs and bubble flow control;
- buffer backpressure and sizing;
- router microarchitecture, buffers, allocators and crossbar;
- performance, power and research directions.

The source defines flow control as the allocation and sharing of network buffers and links. It determines when resources are assigned, at what granularity, and how competing messages make progress. Backpressure is therefore not a property of a bare structural link; it is a channel/router resource-management mechanism.

The source also distinguishes protocol-level deadlock from routing/flow-control deadlock. A routing algorithm can be deadlock-free while a higher-level request/response protocol still forms a cyclic dependency across message classes.

#### ArchLab value

This is the primary NoC textbook and the main source for separating CHI/coherence semantics from the detailed network implementation.

#### ArchLab design implications

The eventual hierarchy should be:

```text
Protocol message
    opcode + line/address + identities + semantic class
        ↓ packetization at network interface
Packet
    destination/routing metadata + one or more flits
        ↓ router/link transport
Flit
    VC/buffer/credit/arbitration state
```

From this follow several project decisions:

1. A `Transaction` or coherence message is not itself a flit.
2. Packetization and reassembly belong in a network interface/adapter.
3. VC occupancy, credit counts, switch allocation and router pipeline state must not leak into the coherence state machine.
4. Protocol message classes need an explicit mapping to transport classes/virtual networks so that protocol-level dependencies can be reviewed.
5. Backpressure at a router or channel must not be misreported as protocol-level `RetryLater` unless the receiver truly did not accept the protocol request.
6. NoC topology, routing, flow control and router microarchitecture should be independently replaceable where practical.

#### Do not copy literally

- Do not implement every topology, routing algorithm or flow-control scheme in the first NoC model.
- Do not assume that adding VCs automatically solves protocol deadlock.
- Do not make the generic `Link` class own all queue, credit and arbitration semantics.
- The first NoC can use a simpler message/packet timing model before introducing flit-level routers.

#### Targeted reading plan

- **Before CHI–NoC boundary:** Chapter 2.
- **Before first NoC:** Chapters 3–5.
- **Before detailed router:** Chapter 6 and related implementation chapters.
- **Before deadlock sign-off:** routing dependency, flow-control dependency and protocol-level dependency sections together.

#### Source locator

- File: `On-Chip Networks Second Edition (Natalie Enright Jerger, Tushar Krishna etc.) (Z-Library).pdf`
- Drive ID: `1N0NuEpUzszd_MC-6EEn45tz_NQy3HJJv`

---

### NOC-002 — GARNET: A Detailed On-Chip Network Model inside a Full-System Simulator

- **Authors:** Niket Agarwal, Tushar Krishna, Li-Shiuan Peh and Niraj K. Jha
- **Publication inspected:** full paper
- **Raw inventory record:** `K0521`
- **Review level:** L1
- **Priority after review:** S

#### Verified scope

GARNET was built because a simple link-latency/bandwidth network can miss effects caused by:

- input-buffer contention;
- VC and switch arbitration;
- realistic link contention;
- router pipeline bubbles;
- message ordering changes;
- feedback from network timing into synchronization and future traffic generation.

The base model implements a classic virtual-channel router with flit-level buffering and credit-based flow control. Its detailed stages include route computation, VC allocation, switch allocation, switch traversal and link traversal. Head flits establish routing and VC state; body and tail flits follow the reserved path, and the tail releases the VC.

A network interface converts memory-system messages into flits and reconstructs messages at the destination. Cache and memory controllers remain outside the router and communicate through that interface.

The paper’s central methodological result is that a detailed NoC can change full-system conclusions. Network timing influences message arrival order, synchronization and future injection. A trace generated in advance cannot capture this timing feedback because the functional execution does not respond to the changed network timing.

#### ArchLab value

GARNET is the primary implementation-style reference for the future detailed NoC backend and for validating the “local detail, rest fast” strategy.

#### ArchLab design implications

1. Provide at least two NoC fidelity levels:

```text
MessageNoC
    message/packet latency, finite queues and contention

DetailedFlitNoC
    packetization, flits, VCs, credits, buffers,
    route/VC/switch allocation, crossbar and links
```

2. Both backends should present the same protocol-facing network-interface contract.
3. A detailed NoC must run in the same virtual-time causal loop as the memory/coherence system when studying timing feedback.
4. Trace-driven NoC simulation remains useful for router studies, but its conclusions must be labeled as trace-driven and not automatically generalized to full-system behavior.
5. Validation should proceed in layers:
   - router and allocator unit tests;
   - synthetic traffic latency/throughput tests;
   - packet conservation and credit invariants;
   - comparison with a simpler NoC at no-load conditions;
   - full-system workloads to expose timing feedback.

#### Do not copy literally

- The original paper’s exact five-stage organization and round-robin allocators are one design point, not mandatory architecture.
- Modern gem5 Garnet implementations have evolved; this paper supplies concepts and motivation, not the sole code authority.
- Hardware multicast assumptions, topology and cache organization must be chosen explicitly for each experiment.
- Detailed flit simulation should not be required merely to boot Linux.

#### Targeted reading plan

- **Read now:** introduction and base model sections for interface boundaries.
- **Before detailed NoC:** router pipeline, network interface and validation sections.
- **Before publication-quality NoC studies:** full-system comparison and methodology sections.

#### Source locator

- File: `garnet.pdf`
- Drive ID: `1r_CHiVl9n8K6K159B8Bs5x1B-ft_dGMj`

## 2. Cross-source synthesis

The five sources converge on these boundaries:

```text
Architectural memory model
        │ legal software-visible order
        ▼
Core / MMU / atomic and fence behavior
        │ memory operations
        ▼
Cache and coherence protocol
        │ protocol messages and line ownership
        ▼
Network interface
        │ packetization / reassembly / class mapping
        ▼
NoC transport
        │ packet / flit / VC / credit / routing / arbitration
        ▼
Memory controller frontend
        │ request queues / mapping / scheduling
        ▼
Detailed DRAM backend
        │ command legality / banks / rows / refresh / buses
```

The boundaries are deliberately not flattened into one `MemoryAccess` function.

## 3. Immediate implications for M0.3 and M0.4

### M0.3 Transaction / Completion

The current generic transaction should carry stable semantic identity and request/result data. It should not contain:

- an MSHR pointer;
- a directory-entry pointer;
- a DRAM queue pointer;
- a router VC or credit;
- a completion callback whose lifetime assumes one process.

The same `TransactionKey` may be referenced by subsystem-private entries, but those entries are not the public identity.

### M0.4 Queue / Backpressure / Retry

The sources reinforce a distinction among three outcomes:

```text
Accepted
    protocol receiver owns the request and owes completion

RetryLater
    protocol receiver did not accept or retain the request

Transport blocked/backpressured
    a channel/router cannot currently move a packet/flit,
    but this does not by itself revoke protocol ownership
```

Transport congestion and protocol acceptance must not be collapsed into one boolean.

## 4. Cache-miss lifecycle inference for later design

This is an ArchLab project inference, not a direct API copied from one source:

```text
CPU request key A
        ↓ L1 miss
MSHR line X created as primary miss owner
        ↓ lower-level transaction key L
CPU request keys B/C to same line may merge as waiters
        ↓ fill returns for L
cache line and protocol state update
        ↓ fan out completions to A/B/C exactly once
```

Consequences:

- public requester keys and lower-level transaction keys are related but not identical;
- an MSHR may aggregate several public transactions;
- one lower-level completion can release several waiting requests;
- duplicate/unknown completion detection remains necessary at every boundary;
- rollback rules differ before and after lower-level acceptance.

This lifecycle will be specified only when the cache/MSHR milestone begins; it must not be prematurely added to M0.3’s minimal generic transaction.

## 5. Validation ladder

```text
Level 1 — functional memory
    bytes, alignment, permissions, exceptions

Level 2 — transaction lifecycle
    identity, acceptance, completion, retry, ordering

Level 3 — cache/coherence
    stable/transient states, ownership, writeback, invariants,
    litmus tests, deadlock and exactly-once completion

Level 4 — NoC
    packet/flit conservation, VC/credit invariants,
    routing, arbitration, latency and saturation

Level 5 — DRAM
    address mapping, command legality, timing constraints,
    refresh, scheduling and bandwidth/latency calibration

Level 6 — full system
    Linux and workloads with timing feedback across all layers
```

## 6. Batch decision

Batch 3 is complete at L1. The next knowledge-review batch is Performance / Datacenter / Methodology. Memory, coherence and NoC sources return at L2/L3 when their implementation milestones begin.
