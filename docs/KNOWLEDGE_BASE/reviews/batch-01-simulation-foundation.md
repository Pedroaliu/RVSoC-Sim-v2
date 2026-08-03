# ArchLab Knowledge Base - Detailed Review Batch 1

## Review depth

- L0: filename-only coarse classification
- L1: actual TOC/abstract/introduction/key architecture sections inspected
- L2: selected chapters or complete short paper
- L3: implementation extraction, code/API mapping, and project decision

This batch is **L1**. It is not being represented as cover-to-cover reading.

## Batch 1: Simulation foundation and backend contracts

### SIM-001 - Fundamentals of Full-Platform Simulation / Программное моделирование вычислительных систем

- **Inventory records:** K0140-K0153 (main-web + 01..12; template excluded)
- **Verified scope:** Interpreter and architectural state; JIT/binary translation/direct execution; trace-driven simulation; full-platform event scheduling and delayed responses; parallel/distributed simulation and determinism; cycle-accurate modeling; cache modeling; simulator languages; interaction with the external world and virtualization.
- **ArchLab value:** Primary theory spine for the whole simulator. It uniquely connects the fast functional front end, event-driven full-platform model, cycle model, checkpoint/state, virtualization, and external-world integration.
- **Design implications:** Keep one architectural state contract across functional/timing modes; make event ordering deterministic; represent delayed completion explicitly; retain trace and direct-execution front ends as peers rather than forks; design checkpointable state and same-tick ordering before PDES.
- **Do not copy literally:** Do not copy the lecture's example port-delay scheme literally into the current Port/Link API. Use it as a semantic reference and keep ArchLab's typed request/response contract.
- **Targeted reading:** Now: chapters 5, 6, 7, 8, 10, 11. Later: 2-4 for interpreter/JIT and 9 for caches.
- **Priority after reading:** S
- **Project phase:** M0 kernel through functional Linux; revisit at timing/PDES/checkpoint stages
- **Source:** https://drive.google.com/file/d/1szaJCUtTVu6ldBu75jm85NM184k-wgQ-

### SIM-002 - Discrete-Event System Simulation, 5th ed. (Banks, Carson, Nelson, Nicol)

- **Inventory records:** K0124, K0127, K0131, K0134 (multiple editions/languages)
- **Verified scope:** Simulation concepts; general principles; simulation software; queueing/statistical models; random inputs; input modeling; verification, calibration and validation; absolute and relative performance estimation.
- **ArchLab value:** Authoritative DES terminology and model-validation discipline. Most useful for defining system state, events, simulation clock, future-event list, time advance, verification and experiment methodology.
- **Design implications:** Specify EventQueue invariants formally; distinguish model verification from validation; add deterministic regression models and later calibration against hardware/gem5/Ramulator.
- **Do not copy literally:** Manufacturing examples and random-variate machinery are not the core of a deterministic architecture simulator. Use those only for synthetic workload generation and statistical experiments.
- **Targeted reading:** Now: chapters 3, 4 and 10. Later: chapters 6, 11 and 12 for analytical/performance work.
- **Priority after reading:** S
- **Project phase:** M0 EventQueue/validation; later statistics and synthetic traffic
- **Source:** https://drive.google.com/file/d/1x_7trnyWBe8vVnEDHXPgICUKBMw1gswQ

### SIM-003 - System Design with SystemC (Grötker, Liao, Martin, Swan)

- **Inventory records:** K0132
- **Verified scope:** SystemC time model; modules; interfaces, ports and channels; processes/events/sensitivity; models of computation; functional/timed models; parameterized modules/channels; interface/channel design; transaction-level modeling; synchronization; refinement; tracing and debugging.
- **ArchLab value:** Best structural reference for SimObject + typed Port + Link/Channel + process/event composition. It also shows how functional and timed models can share interfaces.
- **Design implications:** Treat Port as an interface endpoint and Link as a communication object; keep timing out of payload semantics; parameterize components through configuration; make tracing/debugging first-class rather than an afterthought.
- **Do not copy literally:** The book predates modern TLM-2.0 and its example two-phase bus is pedagogical. Do not recreate a SystemC kernel or bind ArchLab to SystemC execution semantics.
- **Targeted reading:** Now: chapters 2, 7, 8 and 10. Later: chapters 5, 6 and 9.
- **Priority after reading:** S
- **Project phase:** M0.2-M0.4 interfaces, channels, transaction and tracing
- **Source:** https://drive.google.com/file/d/1tB__1jHnPPB37siBjJ9p46z33uNjYSaB

### SIM-004 - Transaction Level Modeling with SystemC (Ghenassia, ed.)

- **Inventory records:** K0083
- **Verified scope:** TLM history and concepts; modeling techniques; embedded-software development; functional verification; architecture analysis and system debugging; design automation. The central idea is separating function from communication at a programmer-view/timed-programmer-view level.
- **ArchLab value:** Strong methodology source for multi-precision transaction models and executable virtual prototypes. It explains why a shared functional reference should serve software, architecture and verification.
- **Design implications:** Keep MemRequest/MemResponse independent of transport implementation; support untimed and timed transports over one semantic transaction contract; plan adapters and architecture-analysis hooks.
- **Do not copy literally:** This is a 2005, pre-modern-TLM-2.0 source. Concepts are valuable, but old APIs, pass-by-reference payload lifetime and callback-centric completion should not become ArchLab's core contract.
- **Targeted reading:** Now: chapters 1-3 and 6. Later: chapters 4, 5 and 7.
- **Priority after reading:** S
- **Project phase:** M0.3 transaction/completion and later virtual-platform composition
- **Source:** https://drive.google.com/file/d/1AS00-sO5Qd8HRucC57DKBe61lBf_Oi87

### SIM-005 - System Simulation with gem5, SystemC and Other Tools

- **Inventory records:** K0004
- **Verified scope:** SystemC/TLM overview; generic payload; loosely timed versus approximately timed models; gem5 as a linked library; transactors between gem5 and SystemC; explicit mapping of functional/atomic/timing accesses to transport_dbg/b_transport/nb_transport.
- **ArchLab value:** Most direct evidence that different precision modes can be mapped through adapter/transactor boundaries. Highly relevant to future gem5/SystemC/Ramulator integration.
- **Design implications:** Define semantic mode mapping separately from transport code; introduce transactors/adapters at external boundaries; preserve request identity and sender state across conversion; keep the simulator event domain explicit.
- **Do not copy literally:** Do not lock current M0 code to gem5 Packet or TLM generic-payload layouts. Those belong in adapters; ArchLab's transaction types remain the stable internal contract.
- **Targeted reading:** Read now in full; revisit the transactor examples when adding external backends.
- **Priority after reading:** S
- **Project phase:** M0.3 semantics now; external adapter stage later
- **Source:** https://drive.google.com/file/d/1XRv4MRktQki38rHzIeqxpZeS_v41yudg

### SIM-006 - Software and System Development Using Virtual Platforms: Full-System Simulation with Simics

- **Inventory records:** K0010, K0015 (duplicate copies)
- **Verified scope:** Full-system virtual platforms that run unmodified binaries; modeling the software-visible hardware interface; firmware/bootloader/hypervisor/OS/driver/application workflows; repeatability, debugging, automation, multi-board/networked systems and lifecycle use.
- **ArchLab value:** Primary reference for the fast functional/full-system side of ArchLab: software-visible correctness, introspection, repeatability, checkpoints and realistic platform composition.
- **Design implications:** Separate software-visible functional fidelity from microarchitectural timing fidelity; model memory maps, registers, interrupts and devices early enough to boot Linux; expose scripting, inspection and deterministic replay.
- **Do not copy literally:** A fast virtual platform is not automatically a performance model. Do not infer cache/pipeline/NoC performance from a programmer-view functional model.
- **Targeted reading:** Now: introduction, model construction, scripting/debug/test/checkpoint chapters. Later: multicore and distributed use.
- **Priority after reading:** S
- **Project phase:** Functional Linux platform, debug, checkpoint and regression infrastructure
- **Source:** https://drive.google.com/file/d/1n5G9ZzNHmL3TcY5Y9zX47dwJddDJQE4f

### SIM-007 - SimBricks: End-to-End Network System Evaluation with Modular Simulation

- **Inventory records:** K0525
- **Verified scope:** Modular end-to-end co-simulation across host, device and network simulators; natural PCIe/Ethernet boundaries; separate simulator processes; shared-memory queues; pairwise synchronization; distributed proxy; determinism, scalability, transparency and Linux/RTL integration.
- **ArchLab value:** Best long-term reference for mixing QEMU/gem5/Verilator/network/device models without creating a monolith.
- **Design implications:** Keep transaction messages serializable and explicit; attach timing to boundary messages; define natural hardware boundaries; do not depend on in-process pointers/callbacks for completion; preserve deterministic ordering and observability.
- **Do not copy literally:** Do not implement multi-process synchronization during M0.3. First make the single-process protocol correct and serializable; the distributed transport comes later.
- **Targeted reading:** Now: sections 4-6 conceptually. Full implementation study at external co-simulation milestone.
- **Priority after reading:** S
- **Project phase:** Contract influence now; implementation in co-simulation milestone
- **Source:** https://drive.google.com/file/d/1sCGYNdSrtUIk559Q21hxUsLH5Zu7JS_5

### MEM-001 - Ramulator 2.0: A Modern, Modular, and Extensible DRAM Simulator

- **Inventory records:** K0523
- **Verified scope:** C++20 DRAM simulator; Interface/Implementation split; registry-based construction; human-readable configuration; frontend-memory-system-controller-scheduler-device separation; controller plugins; standalone or system-simulator backend; request completion callback.
- **ArchLab value:** Primary memory-backend architecture reference and a strong example of modular C++20 simulator design.
- **Design implications:** Use narrow interfaces with replaceable implementations; keep frontend and DRAM backend decoupled; make policy plugins observe issued commands; maintain a resolved configuration; adapt completion into ArchLab's explicit MemResponse path.
- **Do not copy literally:** Do not import the callback lifetime model into the ArchLab core, and do not add a global registry before component contracts stabilize. Registry/configuration is a composition mechanism, not the transaction protocol.
- **Targeted reading:** Now: sections 2.1 and 2.1.1. Later: DRAM specification and validation sections before the DDR model.
- **Priority after reading:** S
- **Project phase:** M0.3 backend contract influence; detailed use in memory-controller/DDR milestone
- **Source:** https://drive.google.com/file/d/152I3a_1AI3tZKR9vhX3F_bibCaqN2C-o

## Batch-level decision

The eight sources converge on one architecture direction:

1. Keep a stable semantic transaction and architectural-state contract.
2. Put precision and transport behind adapters: functional, atomic/transactional, timing, cycle and external co-simulation.
3. Make completion explicit and serialization-ready; do not make in-process callbacks the permanent protocol.
4. Keep functional correctness separate from performance claims.
5. Build determinism, validation, tracing and checkpointability into the framework early.
6. Delay multi-process/PDES implementation until the single-process protocol is proven.

## Immediate M0.3C implication

`issue()` must create pending state before `send()`. A synchronous response may re-enter and remove that state. After `send()`, the requester must re-find by `TransactionKey`; no iterator/reference may survive the call. This is consistent with the explicit request/response boundary required for later adapters and co-simulation.
