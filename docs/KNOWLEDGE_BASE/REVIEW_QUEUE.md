# Knowledge Review Queue

> Last updated: 2026-08-03

每批完成后立即落库。Triage、canonicalization 和 L1/L2/L3 分开计数。

## Completed

### L1 source batches

- Batch 1 Simulation foundation：8；
- Batch 2 CPU / ISA / OOO / SoC：4；
- Batch 3 Memory / Coherence / NoC：5；
- Batch 4 Performance / Datacenter / Methodology：4；
- Batch 5 Linux MM / VM：4；
- Batch 5B RISC-V Linux boot contracts：6；
- Batch 8 Virtualization / QEMU / KVM / IOMMU：6；
- Batch 9 PCIe / NVMe / SSD / CXL：7。

L1 total: **44 canonical source families**.

### Classification batches

- Batch 6: 43 R records resolved; R=0.
- Batch 7: 156 A raw records → 121 canonical groups.

Current A coverage after Batch 9:

```text
covered-L1:       20 groups / 36 raw records
not-yet-L1:      101 groups / 120 raw records
```

Current manifest: `inventory/a-canonical-groups-v3.md`.

## Batch 9 — PCIe / NVMe / SSD / CXL

Status: **L1 complete**

Artifacts:

- `reviews/batch-09-pcie-nvme-ssd-cxl.md`
- `topics/pcie-storage-cxl.md`
- `decisions/pcie-nvme-ssd-cxl-boundaries.md`
- `inventory/batch-09-pcie-storage-cxl.csv`
- `inventory/normalized-snapshot-v5.md`
- `inventory/a-canonical-groups-v3.md`

Storage / PCIe / NVMe / SSD / USB now has zero not-yet-L1 A groups. One adjacent CXL performance group was also covered.

## Batch 10 — RAS / ECC / Chipkill / fault injection / recovery

Status: **next / 18 A canonical groups**

Review order:

1. error-control coding and ECC foundation sources；
2. DRAM field-failure and multi-bit/symbol-error evidence；
3. Chipkill / SDDC / device-width and rank/channel organization；
4. scrub, patrol, retirement and recovery policy；
5. fault injection and validation methodology；
6. cache, interconnect and system-level reliability sources；
7. mapping to ArchLab RAS counters, fault objects, recovery state machines and AVF/MTTF experiments。

Extraction goals:

- separate physical fault, observed syndrome, architectural error and software-visible report；
- define transient/intermittent/permanent and correctable/uncorrectable/fatal taxonomies；
- preserve device/rank/channel/beat/symbol location；
- define ECC encode/check/correct timing and syndrome state；
- model scrub/repair/retirement as explicit events；
- distinguish fault injection from fault detection and recovery；
- define checkpoint/replay interactions and deterministic test vectors；
- map Chipkill x4/x8 organizations without hardcoding one vendor layout；
- identify public versus restricted vendor sources。

Completion artifacts:

- `reviews/batch-10-ras-ecc-reliability.md`
- `topics/ras.md`
- `decisions/ras-fault-recovery-boundaries.md`
- batch inventory and updated A coverage

## Later milestone batches

1. remaining Memory / Cache / DRAM / NVM / CXL（35）；
2. remaining Simulation & Modeling（11）；
3. remaining Linux / OS（11）；
4. CPU/ISA supplements（8）；
5. Parallel / NUMA / HPC / synchronization（8）；
6. Performance / Datacenter supplements（3）；
7. GPU / NPU / DPU / SIMT（2）；
8. Security / confidential computing（2）；
9. Firmware / UEFI（1）；
10. NoC supplement（1）；
11. RTL / FPGA（1）。

## B policy

429 B raw records remain available. They enter review only to support a milestone, citation chain, workload or unresolved design question.
