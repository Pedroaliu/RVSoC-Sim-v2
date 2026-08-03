# Decision — Linux VM and Full-System Boundaries

> Status: accepted as a roadmap/design constraint  
> Date: 2026-08-03  
> Evidence batch: `reviews/batch-05-linux-mm-virtual-memory.md`

## 1. Decision summary

ArchLab models the hardware/firmware contract required to run Linux. It does **not** reimplement Linux memory-management policy inside simulator components.

```text
simulated hardware mechanisms
    provide architectural behavior and timing

guest Linux
    executes allocation, mapping, CoW, reclaim, migration and OOM policy

optional Linux-aware tooling
    observes and decodes guest state without becoming part of correctness
```

## 2. Architectural versus guest-policy ownership

### ArchLab owns

- architectural address translation;
- privilege and access checks;
- PTE interpretation for the selected ISA/version;
- translation-related architectural state;
- fault/trap cause and fault address;
- physical RAM/ROM and MMIO behavior;
- timer, interrupt and UART device contracts;
- deterministic event ordering;
- optional TLB/page-walker timing models;
- checkpointable model state.

### Guest Linux owns

- `mm_struct` and VMA lifetime;
- anonymous and file-backed allocation policy;
- demand paging and CoW decisions;
- page-table construction and modification;
- page cache and writeback;
- reclaim, swap and OOM;
- NUMA memory policy and migration;
- interrupt and fault service policy.

## 3. Public interfaces must remain implementation-neutral

The following must not appear as stable public transaction identity or required simulator API types:

- `struct mm_struct *`;
- `struct vm_area_struct *`;
- `struct page *` / `struct folio *`;
- Linux page-fault-handler pointers;
- TLB-entry pointers;
- page-walker queue pointers;
- kernel-version-specific flag layouts.

Public model contracts use architectural values and stable IDs. Linux-specific interpretation belongs in versioned adapters.

## 4. Translation backend hierarchy

ArchLab may provide multiple implementations behind a common semantic contract:

```text
FunctionalTranslation
    correct address, permission and fault result

TimedTLBTranslation
    finite TLB, replacement and hit/miss latency

DetailedPageWalker
    explicit walk state, queues and cache/memory traffic

IOMMUTranslation
    device context, translation caches and faults

NestedTranslation
    guest and host translation stages
```

The first Linux milestone requires only the first implementation plus the complete platform-visible contract. More detailed models must preserve the same architectural outcome.

## 5. Page-table memory traffic

Guest page tables reside in simulated memory.

- Functional mode may use an optimized walker if it remains semantically equivalent.
- Detailed mode represents page-table walk references as ordinary memory-system transactions.
- Walk requests can contend in caches, coherence, NoC and DRAM.
- Page-table memory is not a magical side channel outside the normal address space.

## 6. Fault handling boundary

The hardware model terminates its responsibility by producing the specified architectural trap state.

```text
access
    -> translation/permission evaluation
    -> success or architectural fault
    -> trap entry
```

Linux then validates mappings and performs policy. The retried instruction re-enters the ordinary core/MMU path.

The simulator must not directly allocate a Linux page merely because an architectural page fault occurred.

## 7. Checkpoint and fidelity switching

Page tables are guest memory and therefore naturally part of memory checkpoint state. TLBs and walker structures are model state.

Each translation backend must declare one restore policy:

- serialize and restore microarchitectural translation state; or
- flush it at a documented checkpoint/fidelity boundary while preserving architectural correctness.

Silent partial restoration is forbidden.

## 8. Linux-aware observability

Linux-aware decoders may expose process, VMA, page and fault context for Top-down analysis, but:

1. decoders are tied to a named kernel build/configuration;
2. decoder failure cannot affect guest execution;
3. raw architectural events remain available without Linux decoding;
4. derived guest labels retain provenance to the decoder version and evidence source.

## 9. NUMA decision

The first Linux platform is single-core and single-memory-node. NUMA is deliberately deferred.

The future NUMA model must represent topology rather than only applying a global penalty:

```text
source execution node
physical home node
device attachment node
route/distance
```

Guest Linux chooses placement and migration policy; ArchLab models the resulting locality and timing.

## 10. Source-completeness gate

Batch 5 establishes Linux/VM boundaries but does not complete the RISC-V platform contract.

Before implementing the first Linux machine, Batch 5B must identify authoritative, versioned sources for:

- the RISC-V privileged architecture and selected Sv39 behavior;
- trap, interrupt and delegation rules;
- timer and interrupt-controller behavior;
- device-tree bindings;
- OpenSBI interfaces and boot handoff;
- Linux RISC-V boot requirements;
- the selected UART and platform-device register interfaces.

Implementation must not fill these gaps from conversational memory alone.

## 11. Consequences

### Positive

- Linux remains the executable source of truth for its policy;
- functional and timing MMUs can be replaced without changing guest semantics;
- guest-aware visualization does not contaminate simulator correctness;
- later NUMA, IOMMU and nested translation fit the same layering;
- checkpoints have an explicit translation-state policy.

### Costs

- Linux-aware introspection requires versioned decoders;
- detailed page walks need memory-system integration;
- authoritative platform specifications must be reviewed before coding;
- functional shortcuts require differential tests to prove equivalence.
