# Topic Map — Linux, Virtual Memory, and Full-System Integration

> Last updated: 2026-08-03  
> Current review depth: Batch 5 L1

## 1. Why this topic exists

ArchLab does not merely execute isolated instructions. Its first visible full-system milestone is:

```text
self-written RV64 functional core
    -> OpenSBI
    -> single-core Linux
    -> initramfs
    -> UART BusyBox shell
```

That goal requires a precise boundary between:

- architectural virtual-memory behavior;
- simulated physical memory and MMIO;
- firmware/platform contracts;
- guest Linux memory-management policy;
- later timing, NUMA and IOMMU refinements.

## 2. Source hierarchy for this topic

### Linux implementation

Primary:

- Lorenzo Stoakes, *The Linux Memory Manager* (Early Access, 2025)

Use for:

- `mm_struct`, VMAs and process address spaces;
- physical page metadata and allocators;
- `mmap()`, demand paging, CoW and page faults;
- reverse mappings and user-memory access;
- page cache, writeback, reclaim, swap and OOM;
- current Linux terminology and code-reading paths.

Limit:

- exact internals must be rechecked against the selected guest-kernel revision;
- x86-specific examples do not define RISC-V architecture behavior.

### OS concepts

Primary:

- *Operating Systems: Three Easy Pieces* (2018 Version 1.00)

Use for:

- abstraction/mechanism/policy separation;
- address spaces, paging, TLBs and page tables;
- fault timelines and replacement concepts;
- minimal directed experiments and teaching models.

Limit:

- not a Linux implementation guide or ISA specification.

### Hardware/software VM boundary

Primary:

- Bhattacharjee and Lustig, *Architectural and Operating System Support for Virtual Memory*

Use for:

- TLB and page-walker structures;
- permission and PTE metadata;
- translation coherence and page-table memory traffic;
- NUMA and heterogeneous memory;
- DMA, IOMMU, ATS/PASID-style requests, MMIO and nested translation.

Limit:

- architecture-neutral; exact Sv39 behavior must come from authoritative RISC-V specifications.

### NUMA placement

Supporting:

- Christoph Lameter, *Local and Remote Memory: Memory in a Linux/NUMA System*

Use for:

- CPU/memory/device locality;
- local versus remote access;
- memory policies, reclaim and migration;
- topology-aware observability.

Limit:

- 2006 / Linux 2.6-era implementation; concepts are useful, details are historical.

## 3. Stable conceptual layers

```text
User program
    virtual addresses, mappings, faults

Guest Linux
    VMA and memory policy
    page allocation and page-table updates
    reclaim, migration and OOM

RISC-V architectural contract
    privilege
    translation mode
    PTE interpretation
    permission checks
    traps and invalidation instructions

ArchLab microarchitecture
    TLBs
    page-walker caches
    walk queues
    timing and contention

Memory system
    caches / coherence / NoC / DRAM / MMIO
```

The public API between layers must not expose guest-kernel C pointers or microarchitectural queue entries.

## 4. What belongs in the first functional platform

### Required

- physical RAM and boot ROM;
- architectural translation for the selected RISC-V VM mode;
- access permissions and privilege checks;
- trap causes and fault addresses;
- supervisor return and instruction retry;
- CSR state needed by firmware and Linux;
- timer and interrupt delivery;
- UART and DTB-visible platform devices;
- OpenSBI/Linux handoff;
- deterministic trace/debug support.

### Explicitly later

- finite TLB capacity and replacement;
- page-walker pipeline and contention;
- TLB shootdown timing;
- multi-core translation coherence;
- NUMA allocation timing and migration costs;
- page-cache/reclaim performance modeling;
- IOMMU, device TLB, ATS/PRI and shared virtual memory;
- nested translation for virtualization.

## 5. Functional versus timing state

### Architectural state

Must be preserved across all fidelity modes:

- privilege mode;
- translation-control CSRs;
- architectural PTE interpretation;
- access permissions;
- fault cause and fault address;
- memory contents, including page tables;
- architecturally visible invalidation effects.

### Microarchitectural state

Replaceable by model:

- TLB entries and replacement metadata;
- page-walker caches;
- outstanding walk entries;
- walk scheduling and memory requests;
- shootdown latency and acknowledgments;
- translation-related counters and queues.

## 6. Fault timeline

```text
memory instruction
    -> translation lookup
    -> TLB hit or walk
    -> translated access
       OR architectural fault
    -> supervisor trap
    -> guest VMA/access validation
    -> guest allocation/load/CoW/PTE update
    -> invalidation as required
    -> return from trap
    -> retry instruction
```

ArchLab statistics should distinguish at least:

- TLB miss without page fault;
- page-table-walk latency;
- permission fault;
- absent mapping fault;
- guest minor/major fault classification when Linux-aware instrumentation is enabled;
- retry completion.

## 7. Linux-aware observability

Optional guest-aware tooling may decode:

- PID/TGID and process name;
- `mm_struct` identity;
- VMA range and flags;
- anonymous versus file-backed mapping;
- page-fault category;
- PFN and NUMA home node;
- page migration or reclaim event.

These decoders are versioned adapters. The simulator kernel must remain functional if all Linux-aware decoding is disabled.

## 8. NUMA and device locality

Later system models should preserve four independent facts:

```text
execution node
memory home node
I/O device attachment node
interconnect route/distance
```

Useful metrics:

- local/remote references and bytes;
- latency distribution by source/home pair;
- inter-socket traffic;
- page placement and migrations;
- DMA source/destination locality;
- IRQ/worker placement relative to device and memory.

## 9. Validation map

### Unit and directed tests

- page-table leaf and non-leaf interpretation;
- all load/store/fetch permission combinations;
- privilege transitions;
- access/dirty-bit behavior for the chosen architecture mode;
- invalid PTEs and misconfigured mappings;
- invalidation and context changes;
- MMIO excluded from ordinary RAM translation where required by platform mapping.

### Differential/reference tests

- Spike/NEMU/QEMU for ISA and privileged behavior where compatible;
- small bare-metal supervisor page-table tests;
- OpenSBI console and timer tests;
- Linux boot and initramfs shell;
- user-space `mmap()`, fork/CoW and protection tests.

### Timing calibration later

- TLB reach and miss rates;
- page-walk memory references;
- walker concurrency;
- shootdown cost;
- NUMA local/remote latency and bandwidth;
- IOMMU translation throughput.

## 10. Source gap and next review

The current source batch does not authoritatively define the complete RISC-V boot platform. The next source review must identify and version:

- RISC-V privileged specification;
- selected interrupt architecture and timer behavior;
- device-tree bindings;
- OpenSBI interfaces;
- Linux RISC-V boot protocol;
- UART/platform-device specifications.

See `REVIEW_QUEUE.md` Batch 5B.
