# Decision — `archlab-rv64-virt-v0`

> Status: accepted knowledge-base decision; implementation not yet complete  
> Date: 2026-08-03

## Context

ArchLab needs a first full-system machine that is small enough to implement and validate, yet compatible with an existing RISC-V firmware/Linux ecosystem. A vague “RISC-V Linux platform” is not sufficient because reset state, address map, interrupts, DTB and SBI behavior are software-visible contracts.

## Decision

The first machine is a **QEMU-virt-compatible subset** named:

```text
archlab-rv64-virt-v0
```

It is defined by pinned architectural and boot contracts rather than by copying all of QEMU.

## Required configuration

### CPU and privilege

- RV64;
- M/S/U modes;
- at least I, M, A, Zicsr and Zifencei;
- C/F/D selected consistently with the Linux/OpenSBI binaries;
- Sv39;
- one hart for Linux-v0.

### Address map

```text
0x00001000  MROM/reset ROM
0x02000000  CLINT-compatible timer/software-interrupt area
0x0c000000  PLIC area
0x10000000  NS16550 UART
0x80000000  DRAM
```

The exact region sizes, register behavior and DT compatible strings must be declared in the implementation design before code is accepted.

### Reset and boot

1. Reset begins in M-mode at the MROM reset vector.
2. The reset path identifies the hart and passes the generated DTB address to firmware.
3. OpenSBI or another SEE runs in M-mode.
4. Linux enters in S-mode with:

```text
a0 = boot hartid
a1 = DTB physical address
satp = 0
```

5. Resident firmware memory is reserved in the DTB.

### SBI

Linux-v0 implements:

- Base;
- TIME when Sstc is absent;
- reset/shutdown service;
- platform console support as needed by OpenSBI, while Linux console uses the NS16550 device.

Linux-v1/SMP adds:

- HSM;
- IPI;
- RFENCE.

### Devices

Required:

- timer/software-interrupt source used by OpenSBI;
- PLIC;
- NS16550 UART;
- RAM and ROM;
- generated DTB.

Deferred:

- virtio;
- PCIe;
- IOMMU;
- AIA/APLIC/IMSIC;
- ACPI/UEFI;
- RTC and test device unless needed by the chosen boot flow.

## Consequences

- Linux-v0 is a real full-system milestone, not merely an ELF runner.
- The platform remains small enough for directed testing.
- OpenSBI, QEMU and Linux provide existing validation anchors.
- Omitted devices are absent from the DTB rather than represented by incomplete stubs.
- Future platform variants can extend the same configuration graph without changing core transaction semantics.

## Non-goals

- exact reproduction of every QEMU `virt` implementation detail;
- detailed cache/NoC/DDR timing before functional Linux works;
- SMP before single-hart boot is deterministic;
- Linux-aware decoding as a correctness dependency.

## Validation gates

The decision becomes an implementation baseline only after tests prove:

1. reset and privilege transition;
2. CSR/trap/Sv39 directed behavior;
3. OpenSBI entry and SBI probing;
4. generated DTB matches the instantiated map;
5. timer and UART interrupt paths;
6. Linux reaches a BusyBox shell;
7. the boot is deterministic and traceable;
8. pinned reference versions are recorded in run metadata.

## Sources

- RISC-V Privileged ISA `v20260120`, Supervisor and Machine ISA version 1.13;
- RISC-V SBI v3.0, ratified 2025-07-16;
- upstream Linux RISC-V boot requirements;
- upstream OpenSBI platform requirements;
- Devicetree Specification;
- upstream QEMU `virt` documentation and source.
