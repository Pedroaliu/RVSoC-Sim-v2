# Detailed Review Batch 5 — Linux MM, Virtual Memory, and Full-System Boundaries

> Review level: **L1**  
> Date: 2026-08-03

L1 means that the actual table of contents, front matter, introduction, and selected implementation/architecture sections were inspected. It does not mean cover-to-cover reading. Linux source paths and RISC-V architectural details must still be verified again when the corresponding implementation milestone begins.

## 1. Canonical works reviewed

### LINUX-001 — The Linux Memory Manager

- **Author:** Lorenzo Stoakes
- **Edition inspected:** No Starch Press Early Access, dated 2025-02-07
- **Raw source:** `The Linux Memory Manager (Lorenzo Stoakes) (Z-Library).pdf`
- **Drive ID:** `1-W1UyuJe102FsG83-nIkTzVBBMMibzgb`
- **Review level:** L1
- **Priority after review:** S

#### Verified scope

The inspected Early Access edition is a detailed Linux-kernel implementation guide. Its contents cover:

1. physical memory metadata through `struct page` and `struct folio`;
2. sections, PFNs, nodes, zones, migratetypes, GFP flags, the buddy allocator and per-CPU page lists;
3. kernel virtual memory and page tables;
4. process memory through `struct mm_struct` and `struct vm_area_struct`;
5. memory mapping and VMA manipulation;
6. page-fault handling;
7. reverse mappings;
8. manipulating/pinning user memory;
9. the page cache;
10. writeback;
11. reclaim and memory pressure;
12. swap;
13. the OOM killer;
14. practical memory-management observation and debugging.

The inspected process-memory material makes an essential distinction:

```text
valid virtual range described by a VMA
        !=
physical page already allocated
        !=
page-table entry already present
```

A process can have an allocation represented by a VMA while physical allocation and page-table installation are deferred until access. The source identifies `struct mm_struct` as the address-space object and `struct vm_area_struct` as the description of valid ranges and their attributes. This is the concrete Linux implementation behind demand paging.

The fault path inspected includes the architecture-specific user-fault entry feeding the generic fault machinery:

```text
do_user_addr_fault()
    -> VMA lookup and access checks
    -> handle_mm_fault()
        -> __handle_mm_fault()
        -> handle_pte_fault()
            -> anonymous/file/CoW/swap paths
```

This reinforces that a hardware exception is only the beginning of fault handling. The kernel validates the VMA and access type, allocates or retrieves backing storage, updates page tables, and returns so the instruction can be retried or the process can receive a signal.

The physical-memory chapters explicitly model locality using nodes and zones. On a multi-socket machine, memory access cost depends on the relationship between the executing CPU and the node that owns the memory.

#### ArchLab value

This is the strongest source in the library for understanding **what Linux itself does after the simulated hardware delivers architectural events**.

Direct uses:

- know which software-visible state must be correct for Linux to progress;
- distinguish VMA validity from PTE presence and physical allocation;
- understand anonymous versus file-backed mappings;
- understand demand paging, CoW, swap-in and permission faults;
- define observation hooks for process address spaces, faults, page allocation, reclaim and migration;
- avoid incorrectly implementing Linux allocation policy inside the simulator.

#### Do not copy literally

- The inspected file is an Early Access edition and contains placeholders and moving implementation details.
- Many detailed fault examples are x86-64-specific; they do not define RISC-V trap causes, `satp`, Sv39 PTE semantics or `SFENCE.VMA`.
- Linux internal functions and structures evolve. Source paths and field layouts must be rechecked against the exact guest kernel version used by ArchLab.
- The simulator must not duplicate `mm_struct`, VMAs, the buddy allocator or reclaim policy as host-side shadow Linux structures. Linux should execute these mechanisms inside the guest; ArchLab models the hardware contract and may observe guest state.

#### Targeted reading plan

- **Before first Linux boot:** Chapters 3–6, especially address-space, page-table, VMA and fault concepts.
- **Before Linux-aware observability:** Chapters 4–8 and 14.
- **Before NUMA/tiered-memory studies:** nodes/zones, placement and migration sections.
- **Before storage/page-cache studies:** Chapters 9–12.
- **Before memory-pressure/RAS work:** reclaim, swap and OOM chapters.

---

### OS-001 — Operating Systems: Three Easy Pieces

- **Authors:** Remzi H. Arpaci-Dusseau and Andrea C. Arpaci-Dusseau
- **Primary edition inspected:** Version 1.00, August 2018
- **Related edition:** Version 0.80, May 2014
- **Raw sources:** `ostep.pdf`; `operating systems 3 easy pieces.pdf`
- **Drive IDs:** `1-bSzaGcMoKBfPyN5FaLvQceAuYq80Fnm`; `1T84RIvDSSvVS1RviT4yJao0PBVKoCZAF`
- **Canonical relation:** same work, different versions; not a byte-duplicate assumption
- **Review level:** L1
- **Priority after review:** S

#### Verified scope

OSTEP organizes operating systems around three abstractions:

```text
virtualization
concurrency
persistence
```

For the current ArchLab milestone, the virtual-memory sequence is the important part. The inspected contents and sections cover:

- address spaces and their goals: transparency, efficiency and protection;
- address translation mechanisms;
- paging and page-table organization;
- TLBs, context-switch effects and address-space identity;
- multi-level page tables;
- swapping and memory beyond physical capacity;
- page-fault behavior and replacement policy;
- hardware/OS cooperation;
- short executable examples and small simulators used to reason about state transitions over time.

OSTEP repeatedly separates:

```text
abstraction
    what software is promised
mechanism
    what makes the abstraction possible
policy
    which resource/placement/replacement choice is made
```

That separation maps directly to ArchLab’s design philosophy. A simulator must first provide the hardware mechanisms and architectural events. The guest OS then chooses policies.

#### ArchLab value

OSTEP is the clearest conceptual bridge between the hardware we model and the guest software behavior we observe.

Direct uses:

- build the mental timeline for a load/store/instruction-fetch translation;
- explain TLB hit, TLB miss, page-table walk and page fault without Linux-specific detail;
- design minimal directed tests before booting a complete kernel;
- keep mechanism and policy separate;
- create small educational traces for translation and fault handling.

#### Do not copy literally

- OSTEP is an operating-systems teaching text, not the RISC-V privileged architecture specification.
- Its simplified page-table and TLB examples are pedagogical models.
- Its OS designs do not define the exact behavior of Linux or the exact state machine of an ArchLab MMU.
- The 2014 edition is an older related version; the 2018 version is the primary canonical source for this batch.

#### Targeted reading plan

- Address spaces and goals;
- paging;
- TLBs;
- multi-level page tables;
- beyond physical memory;
- fault and replacement chapters;
- selected process/context-switch material for ASID/TLB reasoning.

---

### VM-001 — Architectural and Operating System Support for Virtual Memory

- **Authors:** Abhishek Bhattacharjee and Daniel Lustig
- **Publication:** Synthesis Lectures on Computer Architecture, 2017/2018
- **Raw sources:** `Architecture and Operating System Support for Virtual Memory.pdf`; `Architectural and Operating System  Support for Virtual Memory.pdf`
- **Drive IDs:** `17Z-7SshrH-BwTur63aL_3YniDBh9DcBc`; `1T5GhCpa_v4ClH1_3bszJrt9IDj02g4CY`
- **Canonical relation:** title variants and different file sizes; probable related copies, content/hash verification still required
- **Review level:** L1
- **Priority after review:** S

#### Verified scope

This source provides an end-to-end hardware/software view of virtual memory. The inspected contents cover:

- virtual address spaces, permissions, shared memory, synonyms and homonyms;
- paging, page tables and TLBs;
- multi-level TLBs, page sizes, PTE metadata and page-table walkers;
- hardware-managed and software-managed translation;
- translation coherence and OS interaction;
- NUMA and heterogeneous memory;
- DMA, IOMMUs and MMIO;
- device TLBs, ATS/PASID-style translation requests and device page faults;
- nested and shadow page tables for virtualization.

The book’s central architectural lesson is that efficient VM requires hardware/software cooperation. Translation is not merely a function from virtual to physical address: it includes protection, context identity, caching, invalidation, fault delivery, page-table memory traffic and interaction with heterogeneous devices.

The IOMMU case study is especially relevant to future ArchLab work. It separates:

```text
CPU MMU / TLB
Device TLB
IOMMU translation cache and walker
OS-managed page tables and fault handling
PCIe translation/fault messages
```

#### ArchLab value

This is the primary boundary source for a replaceable translation subsystem.

Direct uses:

- define the public MMU request/result contract;
- separate translation semantics from TLB and page-walker timing;
- plan ASID/context-aware TLB state;
- make page-table walks ordinary memory-system traffic in detailed modes;
- preserve permission and fault information across the core/MMU boundary;
- prepare later IOMMU, ATS, PASID, PRI/page-request and accelerator shared-VM models;
- plan nested translation without contaminating the first Sv39 implementation.

#### Do not copy literally

- The source is architecture-neutral with examples from several commercial systems; it does not specify RISC-V Sv39 bit-for-bit behavior.
- AMD/x86 GPU-IOMMU examples are useful architectural cases, not the exact future RISC-V IOMMU implementation.
- Nested/shadow paging and accelerator VM are later milestones and must not expand the first Linux-boot scope.

#### Targeted reading plan

- **First functional MMU:** Chapters 1–4.
- **Timing MMU:** TLB hierarchy, walker and translation-coherence sections.
- **NUMA/tiered memory:** Chapter 7 memory heterogeneity.
- **IOMMU/accelerators:** DMA, IOMMU, MMIO and device-fault sections.
- **Virtualization:** nested and shadow translation sections.

---

### NUMA-001 — Local and Remote Memory: Memory in a Linux/NUMA System

- **Author:** Christoph Lameter
- **Publication:** SGI, 2006
- **Raw source:** `Local.and.Remote.Memory.Memory.in.a.Linux.NUMA.System.pdf`
- **Drive ID:** `1RxzCOew2xIRMaVgzWWDf-Rce7Jq1vMor`
- **Review level:** L1
- **Priority after review:** A

#### Verified scope

The source explains Linux NUMA memory from the perspective of:

- local versus remote access latency and bandwidth;
- node distance;
- page allocation and locality;
- page cache and memory balancing;
- local reclaim;
- page migration;
- `mbind()`, `move_pages()` and migration controls;
- memory policies and cpusets;
- relationships among CPU placement, memory placement and attached I/O devices.

Its most useful system-level point is that locality has at least three coupled placements:

```text
where the thread executes
where the memory page resides
where the I/O device is attached
```

Optimizing only one can move traffic onto the inter-socket fabric and create latency, bandwidth and stability problems elsewhere.

#### ArchLab value

This source is directly relevant to the user’s real performance-debugging cases and to later multi-socket/NUMA simulation.

Direct uses:

- define local/remote memory statistics;
- represent topology distance separately from a single global latency constant;
- model memory placement and migration as software-visible behavior;
- connect CPU, memory and PCIe-device locality in Top-down visualization;
- design synthetic traffic that distinguishes local, one-hop and farther-node access.

#### Do not copy literally

- The paper describes Linux 2.6-era NUMA implementation and 2006 hardware.
- Kernel implementation details, policy defaults and interfaces must be revalidated against the chosen guest kernel.
- SLIT distance numbers are abstract relative values, not direct nanoseconds or bandwidth ratios.
- The first single-core Linux milestone does not need NUMA support; this source influences later topology and observability design.

## 2. Batch-level architecture conclusions

### 2.1 Hardware mechanism is not Linux policy

The reviewed sources converge on this boundary:

```text
ArchLab hardware model
    address translation
    TLB/page-walker behavior
    permission checks
    architectural fault/trap delivery
    memory and MMIO accesses

Guest Linux
    VMA creation and merging
    anonymous/file-backed policy
    demand allocation
    CoW policy
    page cache/writeback
    reclaim/swap/OOM
    NUMA placement and migration policy
```

ArchLab must run Linux’s policy code rather than reimplementing it as simulator policy.

### 2.2 A page fault is a cross-layer transaction

Conceptual sequence:

```text
instruction issues a memory access
    -> MMU/TLB translation
    -> page-table walk or architectural fault
    -> trap enters supervisor software
    -> Linux validates VMA and access
    -> Linux allocates/loads/copies/maps or rejects
    -> PTE and translation state are updated
    -> return from trap
    -> instruction retries
```

Different points in this sequence belong to different components and should remain observable separately.

### 2.3 Guest page tables are memory; TLBs are model state

- Page tables reside in simulated guest memory and are modified by guest software.
- TLBs and page-walker caches are simulated hardware state.
- A detailed page-table walk issues normal memory transactions and can contend in the cache/NoC/DRAM hierarchy.
- Functional mode may translate more directly, but it must produce the same architectural permissions, addresses and fault outcomes.

### 2.4 Linux-internal structures are observation targets, not public simulator contracts

`mm_struct`, VMA, `struct page`, folios and allocator structures are guest-kernel implementation details. They may be decoded by optional Linux-aware tooling, but core simulation APIs must not depend on their current C layouts.

### 2.5 NUMA locality must remain topology-aware

Later multi-node models need separate information for:

- requesting CPU/node;
- physical-memory home node;
- device attachment node;
- route/distance;
- local versus remote latency and bandwidth;
- migration and policy changes over time.

A single `remote_penalty` constant is insufficient for asymmetric or multi-hop topologies.

## 3. Immediate implications for the ArchLab roadmap

### 3.1 First functional Linux milestone

The minimum platform should implement only what Linux needs to observe correctly:

- architectural load/store/fetch translation;
- privilege and permission checks;
- page-fault/trap reporting and retry;
- physical RAM/ROM and MMIO routing;
- timer, interrupt and UART behavior;
- DTB/OpenSBI/Linux boot handoff;
- deterministic execution and debugging.

Detailed Linux allocator, reclaim, page-cache and NUMA timing are not prerequisites.

### 3.2 Timing and detailed VM are later refinements

After functional Linux boots:

```text
functional translation
    -> finite TLBs
    -> page-walker timing and cache traffic
    -> shootdown/invalidation timing
    -> multi-core translation coherence
    -> NUMA placement and remote memory
    -> IOMMU/device translation
    -> nested translation
```

### 3.3 Required validation layers

1. directed translation tests for address, page size and permissions;
2. directed fault tests for load/store/fetch and privilege combinations;
3. differential tests against Spike/NEMU/QEMU where applicable;
4. small supervisor tests that install PTEs and retry faults;
5. OpenSBI boot;
6. Linux boot to UART BusyBox shell;
7. user programs that exercise `mmap()`, fork/CoW, page faults and file-backed mappings;
8. later NUMA and migration tests.

## 4. Source gap discovered

These four sources establish the OS/VM model boundary, but they do **not** fully define the RISC-V Linux boot contract.

Before claiming the full Linux-integration source set is complete, a follow-up review must cover authoritative sources for:

- RISC-V privileged architecture and the chosen Sv39 version;
- trap/interrupt CSRs and delegation;
- timer and interrupt-controller model;
- device tree bindings;
- OpenSBI firmware interfaces and boot handoff;
- Linux RISC-V boot requirements;
- the exact UART and platform devices selected for the first machine.

This follow-up is queued as **Batch 5B — RISC-V Linux boot contracts**. The gap is recorded deliberately rather than silently filling it with general knowledge.
