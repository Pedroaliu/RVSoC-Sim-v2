# Knowledge Base Status

> Last updated: 2026-08-03

这是 ArchLab 资料工作的跨对话接力点。raw asset、canonical group、triage、L1 review 和工程决定分别记录。

## 1. 全量资产

623 条 PDF raw records：Books 520，Paper 103。

当前优先级：

```text
S:  37
A: 155
B: 429
C:   2
R:   0
Total: 623
```

当前 raw inventory：

- `inventory/normalized-snapshot-v4.md`
- Drive ID `1O1j7YWmhsrpYJt3hA1CY4nEJTuU18Pb9`
- SHA-256 `86b3fbd78508e2832cd08ad82d22b4edea1d0d3ebfade2cad169386829cdf11b`

## 2. L1 内容审阅

现有 **37 个 canonical works/source families** 达到 L1：

- Batch 1 Simulation foundation：8；
- Batch 2 CPU / ISA / OOO / SoC：4；
- Batch 3 Memory / Coherence / NoC：5；
- Batch 4 Performance / Datacenter / Methodology：4；
- Batch 5 Linux MM / Virtual Memory：4；
- Batch 5B RISC-V Linux boot contracts：6；
- Batch 8 Virtualization / QEMU / KVM / IOMMU：6。

Batch 6、7 分别是 R triage 与 A canonicalization，不计入 L1 数量。

## 3. 分类与 canonicalization

### Batch 6 — R complete

```text
43 raw R → 32 canonical groups → R=0
```

### Batch 7 — A canonicalization complete

```text
156 A raw records
→ 121 A canonical groups
→ before Batch 8: 7 covered / 114 not-yet-L1
```

### Batch 8 后的 A coverage

- 当前 manifest：`inventory/a-canonical-groups-v2.md`
- Drive ID：`1F9kmdkoR6RZNfuvZI6s0tadIgUGLYCjF`
- SHA-256：`c04b2b41a0f281cba832a0d84e0c1cd4b0bc10097a9ea2dfd777234f230a1989`

```text
A canonical groups total:       121
L1 covered:                      13 groups / 28 raw records
Remaining not-yet-L1:           108 groups / 128 raw records
```

## 4. Batch 8 — Virtualization / QEMU / KVM / IOMMU

Status: **L1 complete**

Reviewed:

1. RISC-V IOMMU Architecture Specification v1.0.1, ratified 2024-09-11；
2. How to Develop Embedded Software Using the QEMU Machine Emulator；
3. KVM虚拟化技术：实战与原理解析；
4. Hardware Support for Efficient Virtualization；
5. AMD IOMMU Specification Rev 3.10, February 2025；
6. QEMU/KVM源码解析与应用。

Records:

- `reviews/batch-08-virtualization-qemu-kvm-iommu.md`
- `topics/virtualization.md`
- `decisions/virtualization-qemu-kvm-iommu-boundaries.md`
- `inventory/batch-08-virtualization.csv`

RISC-V IOMMU is promoted to S as the authoritative future implementation contract. The other five remain A because they are implementation, comparison or historically versioned sources.

## 5. Accepted virtualization direction

```text
Architectural virtualization
Execution backend
Virtual-platform/device composition
I/O virtualization / IOMMU
Timing backend
```

These layers share semantic contracts but not implementation-private pointers or timing claims.

### QEMU/KVM

- QEMU: machine/address-space/device composition and userspace exit handling；
- KVM: hardware-assisted VM/vCPU execution and kernel acceleration；
- KVM/TCG are fast functional backends, not cycle models；
- VM-exit host duration is not guest device latency；
- QOM/MemoryRegion/KVM kernel pointers never become ArchLab public identities。

### IOMMU

- requests preserve `TransactionKey`, device identity, optional PASID/process identity, IOVA, access and attributes；
- DDT/PDT/page tables and CQ/FQ/PQ are simulated guest-visible memory；
- IOTLB/context/walker/ATC entries are private state；
- first/second-stage translation, invalidation/fence, ATS, ATC invalidation and PRI have separate semantics and completions；
- queue full, transport backpressure, translation fault and page-request deferral cannot collapse into one retry result。

### Project ownership

- RVSoC-Sim owns RISC-V machine, devices, IOMMU, SoC timing and QEMU/KVM adapters；
- ArchLab-virt owns the self-built VMM/KVM learning and passthrough experiment path；
- shared artifacts are state schemas, transaction/fault types, traces, test vectors and adapters—not a forced monolithic repository。

## 6. Existing cross-topic decisions

Still active:

- stable architectural-state and semantic-transaction contracts；
- functional/timing/cycle backends behind adapters；
- explicit serialization-ready completion；
- architectural state separated from ROB/LSQ/MSHR/IOTLB/walker private state；
- ISA ordering, coherence, NoC and DRAM timing separated；
- every result carries resolved configuration, ROI, fidelity, seed/repetition and metric provenance；
- `archlab-rv64-virt-v0` remains the first full-system platform。

## 7. Next batch

Batch 9: **PCIe / NVMe / SSD / CXL**. It will consume the six currently queued storage/I/O A groups plus directly required specifications and tie them to the Batch 8 IOMMU/device-assignment boundary.

Accurate state:

> All 623 raw assets are normalized; R=0; 37 source families have L1 review; 13 of 121 A groups are covered and 108 remain. Virtualization/QEMU/KVM/IOMMU boundaries are now recorded and source-backed.
