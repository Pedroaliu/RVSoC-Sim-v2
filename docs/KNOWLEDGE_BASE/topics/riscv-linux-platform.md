# Topic Map — RISC-V Linux Platform

> Last updated: 2026-08-03

## 1. Purpose

This topic map connects the architectural, firmware, boot and device contracts needed to run Linux on ArchLab. It deliberately separates normative specifications from compatibility implementations.

## 2. Source hierarchy

```text
RISC-V Privileged ISA
    normative privilege, CSR, trap, interrupt and Sv39 semantics

RISC-V SBI Specification
    normative S-mode/SEE call boundary

Linux RISC-V Boot Requirements
    normative guest-kernel entry expectations

Devicetree Specification + bindings
    normative hardware-description format

OpenSBI
    reference SEE/firmware implementation

QEMU virt / Spike / NEMU
    executable compatibility and differential references
```

A simulator behavior is not justified solely because QEMU happens to implement it. Architectural behavior must trace to the relevant specification.

## 3. Linux-v0 dependency graph

```text
Reset ROM
    ↓ a0=hartid, a1=dtb, a2=fw_dynamic
OpenSBI / SEE in M-mode
    ↓ SBI Base + TIME + platform operations
Linux entry in S-mode, satp=0
    ↓ setup_vm() / setup_vm_final()
Sv39 + traps + timer + PLIC + UART
    ↓
Initramfs / BusyBox shell
```

## 4. Architectural state

Required state includes:

- current privilege mode;
- `mstatus`, `sstatus` views and interrupt stacks;
- `misa`, `mhartid`;
- `mtvec`/`stvec`;
- `mepc`/`sepc`;
- `mcause`/`scause`;
- `mtval`/`stval`;
- `medeleg`/`mideleg`;
- `mie`/`mip` and `sie`/`sip` views;
- `satp`;
- PMP only when the selected OpenSBI baseline requires protection semantics.

The first implementation can omit optional extensions, but every CSR must have an explicit implemented/WARL/read-only/reserved policy.

## 5. Sv39 model hierarchy

### Functional walker

- immediate page-table traversal through functional memory access;
- exact PTE, permission, superpage, A/D and fault semantics;
- no timing claim.

### Timed walker

- walker requests use ordinary memory transactions;
- page-table cache and outstanding walks are private timing state;
- cache/NoC/DRAM latency feeds back into instruction execution;
- architectural result must match the functional walker.

### Detailed translation hierarchy

Later stages may add:

- split I/D TLBs;
- shared second-level TLB;
- page-walk cache;
- multiple outstanding walks;
- arbitration and backpressure;
- ASID-aware replacement;
- shootdown timing.

## 6. Interrupt path

```text
mtime/time source
    ↓ OpenSBI TIME or Sstc
STIP
    ↓ sie.STIE + sstatus.SIE
S-mode trap
```

```text
UART device
    ↓ source 10
PLIC pending/priority/enable/claim
    ↓ SEIP
S-mode trap
```

Software interrupt and remote-fence paths are added with SMP.

## 7. DTB ownership

The machine builder owns the resolved configuration. DTB generation consumes that configuration and must not duplicate it manually.

Required consistency checks:

- RAM node equals instantiated RAM base/size;
- CPU node count equals hart count;
- ISA/MMU strings match implemented features;
- MMIO `reg` ranges match device mappings;
- interrupt phandles and IDs match wiring;
- reserved firmware memory is excluded from Linux use;
- `/chosen` bootargs and initrd bounds match loaded images.

## 8. Compatibility baseline

`archlab-rv64-virt-v0` uses a QEMU-virt-compatible subset:

```text
MROM       0x00001000
CLINT      0x02000000
PLIC       0x0c000000
UART0      0x10000000, IRQ 10
DRAM       0x80000000
```

Optional devices are not advertised until implemented.

## 9. Validation layers

- ISA-directed tests for CSR/trap/Sv39 rules;
- OpenSBI boot and SBI extension probes;
- DTB schema/semantic checks;
- Linux early-console and timer tests;
- BusyBox shell milestone;
- differential comparison against pinned QEMU/Spike/NEMU versions;
- later timing calibration against gem5 or measured systems where appropriate.

## 10. Observability

Trace should preserve:

```text
virtual address
→ translation lookup/walk
→ PTE reads and result
→ physical address or fault
→ trap delegation
→ firmware/SBI handling when applicable
→ device/interrupt transaction
→ Linux-visible completion
```

Linux-aware labels are optional annotations. They must never be required for correct execution.
