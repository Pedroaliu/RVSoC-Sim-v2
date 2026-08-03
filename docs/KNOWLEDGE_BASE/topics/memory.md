# Topic Map — Memory, Cache and Coherence

> Last updated: 2026-08-03  
> Current depth: L1 source map; implementation details intentionally deferred

## 1. Scope

This topic covers:

```text
Core load/store and instruction fetch
MMU / translation / permissions
Private and shared caches
MSHR / fill / eviction / writeback
Memory consistency
Cache coherence
Directory / Home nodes
Memory controller frontend
DRAM backend
Memory-side statistics and validation
```

NoC transport is documented separately in `noc.md`, even though memory and NoC behavior must be evaluated together.

## 2. Primary sources

### Framework and lifecycle

- `reviews/batch-01-simulation-foundation.md`
  - Transaction/Completion boundary
  - Ramulator 2.0 interface/implementation and backend model
  - explicit, serialization-ready completion

### CPU boundary

- `reviews/batch-02-cpu-isa-ooo.md`
  - architecture versus microarchitecture state
  - LSQ, commit and precise recovery
  - memory completion versus instruction commit

### Memory hierarchy

- `reviews/batch-03-memory-coherence-noc.md`
  - *Memory Systems: Cache, DRAM, Disk*
  - *The Memory System*
  - *A Primer on Memory Consistency and Cache Coherence*

## 3. Model boundaries

### 3.1 Architectural memory semantics

Architectural state includes software-visible results:

- register values;
- memory bytes;
- privilege and page-table effects;
- exceptions and fault addresses;
- architecturally defined atomic/fence behavior;
- the chosen RISC-V memory model.

The NoC does not define these semantics.

### 3.2 Cache functional state

Potential functional state includes:

- tag and line address;
- valid/dirty and replacement metadata;
- line data;
- stable coherence state;
- transient coherence state;
- pending fill/eviction/writeback responsibility.

Exact structures are not frozen at L1.

### 3.3 Cache timing resources

Potential timing-only resources include:

- tag/data ports and banks;
- request, fill and writeback queues;
- MSHR capacity;
- pipeline stages;
- arbitration;
- retry/backpressure timing.

Changing these should not silently change the functional result.

### 3.4 Coherence protocol state

Protocol state includes:

- sharer/owner/Home information;
- protocol message and opcode;
- stable and transient state;
- outstanding acknowledgments;
- ordering and dependency requirements;
- retry and conflict resolution.

Protocol state is distinct from router VC/credit/buffer state.

### 3.5 DRAM backend state

A detailed backend may model:

- address mapping;
- controller queues;
- request scheduling;
- channel/rank/bank/row organization;
- open-row state;
- command legality and timing;
- refresh and maintenance;
- data-bus direction and turnaround.

These details must stay behind the memory-backend interface.

## 4. Planned fidelity levels

```text
FunctionalMemory
    bytes and access semantics

FixedLatencyMemory
    deterministic completion delay

QueuedMemory
    finite capacity, queueing, service and backpressure

DetailedDRAMBackend
    controller and device command timing
```

Experiments must record the selected fidelity. A fixed-latency result is not a detailed DRAM result.

## 5. Transaction and MSHR boundary

The generic transaction identity is:

```text
TransactionKey = (SourceId, RequestId)
```

A future cache may maintain an MSHR keyed by a cache-line identity and merge multiple upper-level transactions. Therefore:

- `TransactionKey` is not an MSHR pointer or index;
- a primary miss may create a distinct lower-level request;
- secondary misses may wait on the same line;
- one fill may produce several upper-level completions;
- every public transaction still completes exactly once;
- lower-level acceptance and upper-level ownership must be tracked explicitly.

This is a design direction only. The cache lifecycle is not part of M0.3.

## 6. Consistency and coherence

### Consistency

Defines legal software-visible shared-memory behavior.

### Coherence

Coordinates cached copies and line ownership in support of the chosen memory model.

### Enforcement chain

```text
program and ISA memory model
        ↓
core pipeline, LSQ, atomics and fences
        ↓
cache/coherence protocol
        ↓
protocol messages
        ↓
NoC transport
```

A correct coherence protocol alone does not guarantee the correct memory model if the core exposes illegal reorderings.

## 7. Initial implementation sequence

```text
Functional RAM
→ minimal cache with functional hit/miss/fill
→ finite MSHR and writeback lifecycle
→ simple multicore coherence protocol
→ directory/Home model
→ message-level NoC timing
→ detailed NoC option
→ queued memory controller
→ Ramulator 2.0 or self-written detailed DRAM backend
```

This sequence is subordinate to the project roadmap. It does not move cache/coherence ahead of the current M0.3 milestone.

## 8. Validation map

### Functional memory

- load/store sizes and alignment;
- byte enables;
- permission and page-fault behavior;
- ROM/RAM/MMIO map;
- differential checks against Spike/NEMU/QEMU where appropriate.

### Cache

- hit/miss and replacement traces;
- dirty eviction and writeback;
- primary/secondary miss behavior;
- queue-full and retry behavior;
- exactly-once completion after merged fills.

### Consistency/coherence

- litmus tests for the selected RISC-V memory model;
- single-writer/multiple-reader and data-value invariants;
- transient-state races;
- retry and acknowledgment races;
- protocol deadlock/livelock tests;
- formal or model-checking support at a later milestone.

### DRAM

- command legality;
- timing-constraint unit tests;
- address-mapping tests;
- row-hit/miss/conflict behavior;
- refresh interactions;
- latency and bandwidth calibration.

## 9. Metrics

Planned metrics include:

- hit/miss by level and cause;
- primary versus merged misses;
- MSHR occupancy and full cycles;
- queue occupancy and wait time;
- eviction/writeback traffic;
- coherence messages and retries;
- NoC transit and blocking time;
- controller queue time;
- DRAM command/service time;
- latency distribution, not only average latency.

## 10. Deferred topics

- full CHI implementation;
- advanced replacement and prefetching;
- NUMA and memory tiering;
- CXL memory pooling;
- persistent-memory ordering;
- RowHammer and DRAM RAS plugins;
- CPU–GPU coherent shared virtual memory.

The sources are already indexed, but detailed reading begins near the relevant milestone.
