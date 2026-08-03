# Topic Map — NoC and Interconnect

> Last updated: 2026-08-03  
> Current depth: L1 source map; detailed router implementation deferred

## 1. Scope

This topic covers the transport layer between SoC components:

```text
Core/cache controllers
Directory/Home nodes
Memory controllers
PCIe/CXL/accelerator adapters
Network interfaces
Packetization and reassembly
Topology and routing
Flow control and backpressure
Virtual channels and credits
Router microarchitecture
NoC timing, power and validation
```

Coherence semantics are documented in `memory.md`; NoC transport must support protocol requirements without owning the protocol itself.

## 2. Primary sources

- *On-Chip Networks, Second Edition*
  - architecture interface, topology, routing, flow control, deadlock and router microarchitecture
- *GARNET: A Detailed On-Chip Network Model inside a Full-System Simulator*
  - flit-level detailed router and full-system timing feedback
- *Modern System-on-Chip Design on Arm*
  - AMBA/AXI/CHI integration, protocol adapters, VC/credit and ESL abstraction
- *A Primer on Memory Consistency and Cache Coherence*
  - protocol ordering, directory state and protocol-level deadlock
- SimBricks
  - future external boundaries and cross-process message transport

Detailed notes are in:

- `reviews/batch-02-cpu-isa-ooo.md`
- `reviews/batch-03-memory-coherence-noc.md`

## 3. Layering

```text
Protocol transaction/message
    semantic opcode, line/address, source/destination role,
    transaction context and ordering class
        ↓
Network interface / adapter
    destination lookup, class mapping,
    packetization, reassembly and injection control
        ↓
Packet
    route-level transport unit
        ↓
Flit
    flow-control unit
        ↓
Router / link
    buffers, VC, credits, routing, arbitration,
    switch traversal and link traversal
```

The same protocol-facing interface should support a simple message-level NoC and a detailed flit-level NoC.

## 4. Planned fidelity levels

### MessageNoC

Models:

- finite input/output queues;
- configurable per-hop or end-to-end latency;
- bandwidth and arbitration;
- message/packet contention;
- backpressure;
- topology and destination routing at a coarse level.

Does not claim:

- flit-level buffer behavior;
- VC allocator fidelity;
- switch allocator fidelity;
- detailed router pipeline timing.

### DetailedFlitNoC

Adds:

- packet-to-flit conversion;
- input buffers;
- route computation;
- VC allocation;
- switch allocation;
- crossbar traversal;
- link traversal;
- credit return;
- head/body/tail behavior;
- detailed occupancy and contention.

The exact pipeline depth is configurable and must not be hard-wired to one historical GARNET design.

## 5. Protocol versus transport ownership

### Protocol owns

- request/response semantics;
- line or address ownership;
- Home/directory state;
- acknowledgments and completion conditions;
- ordering rules;
- protocol retry and conflict handling;
- protocol-level dependency analysis.

### NoC owns

- packet/flit storage;
- routing decisions;
- physical/logical output selection;
- VC and buffer allocation;
- credits and channel availability;
- switch and link timing;
- transport backpressure;
- transport-level deadlock avoidance.

### Adapter owns

- protocol-message to transport-class mapping;
- packetization and reassembly;
- destination calculation;
- serialization of protocol metadata;
- conversion between message-level and flit-level backends.

## 6. Deadlock taxonomy

Three classes must be reviewed separately:

```text
Routing deadlock
    cyclic channel dependencies created by routing choices

Flow-control deadlock
    cyclic buffer/resource dependencies in the transport

Protocol deadlock
    request/response/message-class dependencies across the protocol
```

A deadlock-free routing algorithm does not prove the protocol is deadlock-free. Adding VCs also does not automatically solve protocol cycles; the mapping from protocol classes to transport resources must be justified.

## 7. Backpressure and RetryLater

The transport may be blocked while the protocol receiver already owns a transaction. Therefore:

```text
Transport blocked
    packet/flit waits in a queue or router
    protocol ownership unchanged

RetryLater
    protocol target did not accept or retain the request
    requester retains ownership and may retry
```

The generic Link remains a structural connection. Queue capacity, arbitration, credits and delayed delivery belong in channel/NoC components.

## 8. Router model direction

A future baseline detailed router should contain explicit state for:

- per-input or shared buffers;
- virtual-channel state;
- route result;
- VC allocator requests/grants;
- switch allocator requests/grants;
- crossbar movement;
- output credits;
- link pipeline;
- packet completion and VC release.

Current/next state separation will be required to avoid update-order artifacts in cycle simulation.

## 9. Timing feedback

GARNET demonstrates why a detailed NoC sometimes must run inside the full-system causal loop:

```text
NoC delay changes response arrival
→ synchronization and cache state evolve differently
→ cores generate later traffic at different times
→ injection pattern changes
```

A pre-generated trace cannot reproduce this feedback. Trace-driven NoC remains valid for many isolated network questions, but its scope must be declared.

## 10. Validation map

### Structural

- all endpoints connected exactly once where required;
- legal routes exist;
- packet destination and reassembly correctness;
- no packet/flit duplication or loss.

### Flow control

- credit conservation;
- buffer occupancy bounds;
- VC ownership and release;
- no movement without downstream capacity;
- tail-flit release exactly once.

### Routing and deadlock

- deterministic-route unit tests;
- adaptive-route legality tests when added;
- channel-dependency analysis;
- protocol-message-class dependency analysis;
- stress tests near saturation.

### Performance

- no-load latency;
- accepted throughput;
- latency-throughput curve;
- fairness and starvation;
- queue/VC occupancy;
- per-stage stall attribution;
- comparison between MessageNoC and DetailedFlitNoC at compatible configurations.

### Full system

- cache/coherence traffic;
- synchronization-sensitive workloads;
- shared versus private LLC studies;
- effect of network fidelity on system-level conclusions.

## 11. Metrics

Planned metrics include:

- messages, packets and flits by class;
- injection/ejection rate;
- end-to-end and per-hop latency distribution;
- input-buffer and VC occupancy;
- credit stalls;
- route/VC/switch allocation wait;
- link utilization;
- head-of-line blocking;
- protocol-class interference;
- energy/power estimates at a later stage.

## 12. Initial implementation order

```text
Typed protocol message
→ network interface and destination mapping
→ MessageNoC with finite queues and latency
→ topology/routing abstraction
→ protocol-class mapping and deadlock review
→ packet/flit representation
→ detailed VC router
→ Garnet-like full-system timing studies
```

This order does not move NoC implementation ahead of the current simulator-kernel milestones.
