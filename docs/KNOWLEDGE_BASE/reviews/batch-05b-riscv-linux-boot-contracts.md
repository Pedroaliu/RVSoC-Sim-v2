# Detailed Review Batch 5B — RISC-V Linux Boot Contracts

> Review level: **L1**  
> Date: 2026-08-03

This batch closes the source gap recorded by Batch 5. It uses authoritative specifications, upstream Linux documentation, OpenSBI documentation, the Devicetree specification, and upstream QEMU source/documentation. L1 means the relevant normative sections, boot requirements, platform requirements, and implementation entry points were inspected. It does not replace L2/L3 implementation work or conformance tests.

## 1. Source families reviewed

### RISCV-BOOT-001 — RISC-V Privileged Architecture

- **Source:** RISC-V Ratified Specifications Library
- **Selected snapshot:** Privileged ISA `v20260120`, January 2026
- **Relevant chapter:** Supervisor-Level ISA version 1.13 and Machine-Level ISA version 1.13
- **Priority:** S
- **Role:** normative architectural contract

#### Verified scope

The selected source defines:

- reset entering M-mode;
- M/S/U privilege state;
- trap CSRs and `MRET`/`SRET` state transitions;
- interrupt enable/pending behavior;
- trap delegation;
- `satp`, ASID and translation mode selection;
- `SFENCE.VMA` ordering and local invalidation semantics;
- Sv39 virtual-address canonicality;
- three-level page-table traversal;
- 4 KiB pages, 2 MiB megapages and 1 GiB gigapages;
- PTE validity, permissions, user/global/accessed/dirty behavior;
- page-fault outcomes.

#### ArchLab implications

1. `satp`, page tables and `SFENCE.VMA` are architectural semantics, not optional Linux-specific conveniences.
2. Functional and timed walkers must return the same translation, permission and exception result.
3. A write to `satp` is not automatically a global TLB flush.
4. `SFENCE.VMA` is local to one hart; remote shootdown requires an IPI/SBI path and a local fence on each remote hart.
5. The first implementation may conservatively over-fence, but the externally visible behavior must remain legal.
6. A/D behavior must be an explicit configuration. The implementation must not silently mix fault-on-clear and hardware-update schemes.

### RISCV-BOOT-002 — RISC-V Supervisor Binary Interface Specification

- **Source:** RISC-V Ratified Specifications Library
- **Version:** SBI v3.0, ratified 2025-07-16
- **Priority:** S
- **Role:** normative S-mode/SEE interface

#### Verified scope

The SBI specification defines the execution-environment boundary used by S-mode or VS-mode software. The binary calling convention uses `ECALL`; modern extensions encode EID in `a7`, FID in `a6`, arguments in `a0`–`a5`, and return the error/value pair in `a0`/`a1`.

The current ratified specification includes the Base extension and modular services such as TIME, IPI, RFENCE, HSM, SRST, PMU and DBCN.

#### ArchLab staged baseline

For **single-hart Linux-v0**:

- Base;
- TIME when Sstc is not modeled;
- SRST for clean shutdown/reset support;
- platform console through NS16550 MMIO; DBCN may be added for firmware/debug use.

For **SMP Linux-v1**:

- HSM;
- supervisor IPI;
- remote fence/RFENCE;
- the single-hart services above.

The implementation must expose a discoverable SBI version and extension set. Legacy SBI v0.1 calls are not the permanent ArchLab contract.

### RISCV-BOOT-003 — Linux RISC-V Kernel Boot Requirements and Constraints

- **Source:** upstream Linux kernel documentation
- **Snapshot:** `docs.kernel.org/next`, inspected 2026-08-03
- **Priority:** S
- **Role:** normative guest-kernel handoff contract

#### Verified handoff requirements

At Linux entry:

```text
a0 = boot hartid
a1 = physical address of DTB
satp = 0
```

Additional requirements include:

- resident firmware/PMP-protected memory must be marked reserved;
- RV64 kernel placement is expected at a 2 MiB PMD boundary;
- firmware passes Devicetree or ACPI hardware description;
- ordered SMP boot through SBI HSM is preferred;
- early virtual mapping is established in two stages by `setup_vm()` and `setup_vm_final()`.

#### ArchLab implications

The simulator does not fabricate Linux internal state. It must deliver the documented entry state, memory image and hardware description, then allow Linux to create its own mappings and policy state.

### RISCV-BOOT-004 — OpenSBI Platform Requirements and Firmware Model

- **Source:** upstream `riscv-software-src/opensbi`
- **Snapshot:** repository `master`, inspected 2026-08-03
- **Priority:** S
- **Role:** reference firmware/platform integration

#### Verified scope

OpenSBI is a reference implementation of the SBI execution environment. Its platform requirements include:

- at least RV32IMA_Zicsr or RV64IMA_Zicsr on all harts;
- at least one hart with S-mode;
- direct-mode `mtvec` support;
- optional PMP, with loss of firmware protection if absent;
- a hardware `time` source or 64-bit MMIO counter;
- a mechanism to inject M-mode software interrupts on multi-hart systems.

OpenSBI separates platform-independent SBI logic from platform operations such as timer, IPI, console and reset handling.

#### ArchLab implications

1. Do not embed Linux boot policy in the CPU model.
2. Provide the machine mechanisms OpenSBI expects.
3. Keep the platform operation boundary explicit so OpenSBI, a minimal in-house SEE, or a test firmware can share the same machine.
4. Pin a concrete OpenSBI release or commit when the executable Linux milestone starts; this L1 source snapshot is not yet a dependency lock.

### RISCV-BOOT-005 — Devicetree Specification

- **Source:** `devicetree-org/devicetree-specification`
- **Snapshot:** upstream repository, inspected 2026-08-03
- **Priority:** S
- **Role:** hardware-description contract

#### Verified scope

A compliant tree has a root node, one `/cpus` node and at least one `/memory` node. Memory ranges are described with `reg`; `/reserved-memory` excludes regions from normal OS use and can mark `no-map` regions. Device nodes describe compatible strings, registers, interrupt relationships and topology.

#### Required Linux-v0 DTB content

The first ArchLab DTB must describe at least:

- root `compatible`, address and size cell widths;
- `/cpus` with hart nodes, ISA/MMU description and interrupt-controller children;
- `/memory@80000000` with actual RAM size;
- `/reserved-memory` for resident firmware/ROM regions that Linux must not use;
- `/chosen` with boot arguments and optional initrd bounds;
- `/soc` with timer/software-interrupt block, PLIC, NS16550 UART and their interrupt wiring.

The DTB is generated from resolved machine configuration. It is not a second independent configuration file that may drift from the instantiated platform.

### RISCV-BOOT-006 — QEMU `virt` Platform Reference

- **Source:** upstream QEMU documentation and source (`docs/system/riscv/virt.rst`, `hw/riscv/virt.c`, `hw/riscv/boot.c`)
- **Snapshot:** QEMU `master`, inspected 2026-08-03
- **Priority:** S as a compatibility reference, not a normative ISA source
- **Role:** executable reference platform and differential target

#### Verified platform facts

Current QEMU `virt` documentation describes a generic virtual platform with CLINT/ACLINT options, PLIC or AIA options, NS16550 UART, virtio-mmio, PCIe and an automatically generated DTB.

The inspected source defines the important address map entries:

```text
0x00001000  MROM/reset area
0x00100000  test device
0x00101000  RTC
0x02000000  CLINT/ACLINT timer-software-interrupt area
0x0c000000  PLIC/APLIC area
0x10000000  NS16550 UART
0x10001000  first virtio-mmio window
0x20000000  flash
0x30000000  PCIe ECAM
0x40000000  PCIe MMIO
0x80000000  DRAM base
```

The reset ROM loads `a0` from `mhartid`, loads `a1` with the DTB address, prepares OpenSBI dynamic information in `a2`, and jumps to the selected firmware entry. QEMU source also aligns RV64 kernels to 2 MiB and places the DTB in RAM away from the kernel/initrd.

#### ArchLab use

QEMU `virt` is the first interoperability target because it provides a widely used firmware/Linux environment. ArchLab does **not** claim byte-for-byte identity with every QEMU device. The first machine is a named compatible subset whose differences are explicit.

## 2. ArchLab Linux-v0 machine decision

The first full-system machine is named:

```text
archlab-rv64-virt-v0
```

It is a **QEMU-virt-compatible subset**, with this staged scope:

### Required in Linux-v0

- one RV64 hart;
- M/S/U privilege modes;
- RV64IMA_Zicsr_Zifencei baseline, with C/F/D enabled according to the selected Linux build;
- Sv39;
- reset ROM at `0x00001000`;
- resident OpenSBI or test SEE loaded in DRAM;
- DRAM at `0x80000000`;
- CLINT-compatible or ACLINT-backed machine timer/software interrupt mechanism at the QEMU-compatible region;
- PLIC at `0x0c000000`;
- NS16550-compatible UART at `0x10000000`, interrupt source 10;
- generated DTB;
- Linux entry with `a0=hartid`, `a1=dtb_pa`, `satp=0`;
- SBI Base and TIME, plus reset/shutdown support.

### Deferred to Linux-v1/SMP

- multiple harts;
- HSM, IPI and RFENCE;
- coherent private caches and timed page-table walks;
- virtio block/network;
- PCIe and IOMMU;
- AIA/APLIC/IMSIC;
- ACPI/UEFI.

## 3. Layer boundaries

```text
RISC-V ISA
    privilege, CSR, trap, interrupt and Sv39 semantics

Platform machine
    ROM, RAM, UART, timer/software interrupt, PLIC and DTB

SEE / OpenSBI
    M-mode platform control and SBI services

Linux
    S-mode kernel, page tables, processes, drivers and policy
```

No layer may reach into another layer’s private state to shortcut correctness.

## 4. Validation matrix

### Directed ISA tests

- `satp` Bare/Sv39 switching;
- canonical and non-canonical Sv39 addresses;
- all PTE permission combinations used by Linux;
- leaf at each Sv39 level and misaligned superpage faults;
- A/D behavior selected for the configuration;
- `SFENCE.VMA` forms and conservative over-fencing;
- trap entry, delegation and return;
- timer, software and external interrupt enable/pending transitions.

### Firmware tests

- reset at MROM;
- correct `mhartid` and DTB pointer handoff;
- OpenSBI boot banner and extension probing;
- timer event delivery;
- UART polled and interrupt-driven operation;
- clean reset/shutdown.

### Linux milestones

1. OpenSBI enters Linux.
2. Linux prints through early console.
3. Memory and DTB are discovered correctly.
4. Timer interrupts advance jiffies.
5. PLIC/UART interrupt path works.
6. Initramfs reaches a BusyBox shell.
7. A deterministic boot trace can be compared with QEMU/Spike/NEMU at defined boundaries.

## 5. Do not copy blindly

- QEMU source is a compatibility reference, not an architectural specification.
- QEMU `master` can change; implementation baselines must pin a version/commit.
- OpenSBI `master` documentation is not a substitute for a pinned release.
- Linux `next` documentation describes current expectations but the selected guest kernel must also be pinned.
- A full QEMU `virt` clone is unnecessary for Linux-v0; every omitted device must simply be absent from the DTB and documented.

## 6. Batch conclusion

The first Linux platform now has a source-backed contract. The remaining work is implementation and L2/L3 extraction, not guessing the boot ABI from memory.
