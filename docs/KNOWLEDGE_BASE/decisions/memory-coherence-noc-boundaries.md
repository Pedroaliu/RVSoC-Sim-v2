# Decision — Memory, Coherence and NoC Boundaries

> Status: accepted as a framework direction; detailed APIs are deferred  
> Date: 2026-08-03

## Context

ArchLab will eventually model cache hierarchy, directory/CHI-like coherence, NoC, memory controllers and detailed DRAM. Without explicit boundaries, these layers tend to collapse into one tightly coupled `access()` path whose internal pointers and timing assumptions leak across the simulator.

Batch 3 sources show that this would make fidelity replacement, validation and external backend integration difficult.

## Decision 1 — Preserve semantic transaction identity across subsystem boundaries

The generic identity remains:

```text
TransactionKey = (SourceId, RequestId)
```

A subsystem may create its own private entries, such as:

- MSHR;
- writeback entry;
- directory transaction;
- Home-node transaction;
- controller queue entry;
- DRAM request;
- packet/flit/VC state.

These private entries may reference a public key or create a child transaction, but none becomes the public identity itself.

### Consequence

No public request or response may depend on an in-process pointer to MSHR, directory, controller or router state.

## Decision 2 — Separate architectural memory semantics from coherence and transport

```text
ISA memory model
    defines legal software-visible behavior

Core/MMU/LSQ/fence/atomic implementation
    helps enforce that behavior

Coherence protocol
    coordinates cached copies and ownership

NoC
    transports protocol messages
```

Coherence alone does not define the memory model, and the NoC cannot repair an illegal ordering exposed by the core.

## Decision 3 — Separate protocol messages from packets and flits

A protocol message contains semantic information. A network interface maps it to transport units.

```text
Protocol message
    ↓ network interface
Packet
    ↓ detailed transport
Flits
```

Packetization, reassembly and transport-class mapping are adapter responsibilities.

### Consequence

Changing from a message-level NoC to a detailed flit-level NoC does not require rewriting the coherence state machine.

## Decision 4 — Distinguish protocol acceptance from transport backpressure

```text
Accepted
    receiver owns request and owes completion

RetryLater
    receiver did not accept or retain request

Transport blocked
    accepted packet/flit cannot currently advance
    ownership does not revert
```

A router queue filling up is not by itself a protocol `RetryLater`.

## Decision 5 — Memory fidelity is explicit experiment metadata

The simulator will distinguish at least conceptually:

- FunctionalMemory;
- FixedLatencyMemory;
- QueuedMemory;
- DetailedDRAMBackend.

Each mode has a declared accuracy scope. Fixed latency may support bring-up and isolation, but it cannot be presented as a detailed DRAM performance result.

## Decision 6 — Cache functional state and timing resources are separable

Cache correctness concerns line data, tag/state, fill, eviction and coherence behavior. Timing concerns ports, banks, queues, MSHR capacity and arbitration.

The project should permit a functional cache model and a timing cache model to share semantic interfaces without requiring identical internal state.

## Decision 7 — Protocol and transport deadlock are checked separately

Review must distinguish:

- routing dependency;
- flow-control/resource dependency;
- protocol/message-class dependency.

VCs and deadlock-free routing are not treated as proof of protocol deadlock freedom.

## Decision 8 — Detailed timing feedback requires integrated virtual time

When the research question concerns NoC or DRAM timing feedback, detailed models must participate in the same causal simulation loop as cores and coherence. Pre-generated traces may be used for isolated studies but must be labeled trace-driven.

## Deferred API questions

The following are intentionally not decided by this document:

- exact cache request/response types;
- MSHR container and key structure;
- stable/transient coherence states;
- CHI channel and opcode mapping;
- number of protocol message classes or virtual networks;
- packet and flit classes;
- router pipeline depth;
- DRAM backend interface beyond explicit request/completion;
- external Ramulator integration details.

These will be decided at their implementation milestones, using this document as a boundary constraint.

## Validation consequences

Future tests must be layered:

1. architectural memory semantics and litmus tests;
2. transaction identity and exactly-once completion;
3. cache/MSHR lifecycle and merge/fan-out;
4. coherence invariants and transient-state races;
5. packet/flit conservation and credit invariants;
6. DRAM command legality and timing;
7. full-system calibration and timing-feedback workloads.

## Sources

- *Memory Systems: Cache, DRAM, Disk*
- *The Memory System: You Can’t Avoid It, You Can’t Ignore It, You Can’t Fake It*
- *A Primer on Memory Consistency and Cache Coherence, Second Edition*
- *On-Chip Networks, Second Edition*
- *GARNET: A Detailed On-Chip Network Model inside a Full-System Simulator*
- Ramulator 2.0 and SimBricks from Batch 1
