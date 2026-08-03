# Knowledge Review Queue

> Last updated: 2026-08-03

Review follows ArchLab milestones. Each completed batch is written to the repository immediately.

## Completed L1 batches

### Batch 1 — Simulation foundation

8 canonical works. Record: `reviews/batch-01-simulation-foundation.md`.

### Batch 2 — CPU / ISA / OOO / SoC boundaries

4 canonical works. Records: `reviews/batch-02-cpu-isa-ooo.md`, `topics/cpu.md`.

### Batch 3 — Memory / Cache / Coherence / NoC

5 canonical works. Records: `reviews/batch-03-memory-coherence-noc.md`, `topics/memory.md`, `topics/noc.md`.

### Batch 4 — Performance / Datacenter / Methodology

4 canonical works. Records: `reviews/batch-04-performance-datacenter-methodology.md`, `topics/performance.md`.

### Batch 5 — Linux MM / Virtual Memory foundation

4 canonical works. Records: `reviews/batch-05-linux-mm-virtual-memory.md`, `topics/linux.md`.

### Batch 5B — RISC-V Linux boot contracts

6 source families:

1. RISC-V Privileged Architecture `v20260120`;
2. RISC-V SBI v3.0;
3. Linux RISC-V boot requirements;
4. OpenSBI platform requirements;
5. Devicetree Specification;
6. upstream QEMU `virt` platform.

Records:

- `reviews/batch-05b-riscv-linux-boot-contracts.md`;
- `topics/riscv-linux-platform.md`;
- `decisions/riscv-linux-machine-v0.md`;
- `inventory/batch-05b-riscv-linux-boot.csv`.

## Batch 6 — R-class manual triage

Status: **next / 43 raw records**

For every R record:

1. identify the actual bibliographic title or paper;
2. inspect first pages, TOC or abstract when filename is insufficient;
3. assign canonical work and domain;
4. choose S/A/B/C;
5. mark duplicate/edition relationships;
6. explain why it was ambiguous;
7. immediately promote newly found S sources.

R triage is complete only when no record remains with `Priority=R`.

## Batch 7 onward — A-class milestone groups

After R triage and normalized inventory recalculation:

1. Virtualization / QEMU / KVM / IOMMU;
2. PCIe / NVMe / SSD / CXL;
3. RAS / ECC / Chipkill / fault injection / recovery;
4. Parallel / NUMA / HPC / synchronization;
5. Compiler / assembly / JIT / binary translation;
6. GPU / NPU / DPU / SIMT / collectives;
7. Power / thermal / energy;
8. Security / confidential computing;
9. Firmware / boot / UEFI;
10. software-engineering sources that directly affect simulator implementation.

The original A pool has 145 raw records, but several have already been absorbed into completed canonical batches. The remaining canonical count will be recalculated after Batch 6.

## B-class references

398 raw records are retained and searchable. They are read when a milestone, a citation chain or an unresolved design question needs them; they are not scheduled for performative cover-to-cover reading.

## Immediate work sequence

```text
43 R records
    ↓
normalized 623-record inventory
    ↓
remaining A canonical groups
    ↓
milestone-driven L1/L2/L3 review
```
