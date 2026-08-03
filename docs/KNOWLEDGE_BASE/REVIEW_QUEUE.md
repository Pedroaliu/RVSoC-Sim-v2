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
- Batch 8 Virtualization / QEMU / KVM / IOMMU：6。

L1 total: **37 canonical source families**.

### Classification batches

- Batch 6: 43 R records resolved; R=0.
- Batch 7: 156 A raw records → 121 canonical groups.

Current A coverage after Batch 8:

```text
covered-L1:       13 groups / 28 raw records
not-yet-L1:      108 groups / 128 raw records
```

Current manifest: `inventory/a-canonical-groups-v2.md`.

## Batch 9 — PCIe / NVMe / SSD / CXL

Status: **next**

Primary remaining A groups in this domain: 6 canonical groups. The batch may pull in directly required B references or current official specifications when an A source alone does not define a software-visible contract.

Expected source families include:

- PCI Express architecture and transaction/credit model;
- PCIe root complex, endpoint, BAR/configuration/MSI/MSI-X and DMA;
- NVMe controller, queue pairs, commands and completions;
- SSD architecture, FTL, flash geometry and latency/parallelism;
- CXL.io / CXL.cache / CXL.mem and device types;
- hotplug, SR-IOV, ATS/PASID/PRI interactions;
- existing RISC-V/AMD IOMMU conclusions from Batch 8.

Extraction goals:

1. separate software-visible protocol state from link/controller timing state;
2. define Requester ID, Tag, PASID and TransactionKey relationships;
3. model posted/non-posted/completion ownership and credit/backpressure;
4. define PCIe configuration and memory-map discovery;
5. define NVMe submission/completion queue semantics and doorbells;
6. separate NVMe controller functional behavior from NAND/FTL timing;
7. define CXL protocol boundaries and coherency/memory semantics;
8. build directed validation from register access through DMA and completion;
9. map device assignment and IOMMU translation from Batch 8;
10. produce staged PCIe/NVMe/CXL implementation milestones.

Completion artifacts:

- `reviews/batch-09-pcie-nvme-ssd-cxl.md`
- `topics/pcie-storage-cxl.md`
- a protocol/timing boundary decision
- batch inventory and updated A coverage

## Later milestone batches

1. RAS / ECC / Chipkill / fault injection / recovery；
2. remaining Memory / Cache / DRAM / NVM；
3. remaining Simulation & Modeling；
4. remaining Linux / OS；
5. Parallel / NUMA / HPC / synchronization；
6. CPU/ISA implementation supplements；
7. Compiler / JIT / binary translation；
8. GPU / NPU / DPU / SIMT / collectives；
9. Power / thermal / energy；
10. Security / confidential computing；
11. Firmware / UEFI；
12. directly relevant software engineering。

## B policy

429 B raw records remain available. They enter review only to support a milestone, citation chain, workload or unresolved design question.
