# Knowledge Review Queue

> Last updated: 2026-08-03

每批完成后立即写入仓库。Priority、triage、canonicalization 和 L1/L2/L3 是不同状态。

## 已完成

### L1 batches

- Batch 1 Simulation foundation：8 canonical works；
- Batch 2 CPU / ISA / OOO / SoC：4；
- Batch 3 Memory / Cache / Coherence / NoC：5；
- Batch 4 Performance / Datacenter / Methodology：4；
- Batch 5 Linux MM / Virtual Memory：4；
- Batch 5B RISC-V Linux boot contracts：6 source families。

L1 合计：**31**。

### Batch 6 — R-class triage

- 43 raw records 已全部处理；
- 32 canonical groups；
- R=0。

### Batch 7 — A-level canonicalization

- 156 A raw records；
- 121 A canonical groups；
- 7 groups 已被既有 L1 batches 覆盖；
- **114 not-yet-L1 A groups** 形成当前真实队列。

记录：

- `reviews/batch-07-a-canonicalization.md`
- `inventory/a-canonical-groups-v1.md`
- `inventory/normalized-snapshot-v3.md`

## Batch 8 — Virtualization / QEMU / KVM / IOMMU

Status: **next**

六个未审 A canonical groups：

1. RISC-V IOMMU specification/reference material；
2. How to Develop Embedded Software Using the QEMU Machine Emulator；
3. KVM 虚拟化技术：实战与原理解析；
4. Hardware Support for Efficient Virtualization；
5. AMD IOMMU；
6. QEMU/KVM 源码解析与应用。

Supporting sources may include already reviewed Simics/VM/RISC-V platform work and B-level virtualization texts when they close a concrete gap.

提取目标：

- trap-and-emulate、direct execution、paravirtualization 与 hardware assist；
- guest architectural state、VM-exit reason 与 host control state；
- GVA→GPA→HPA / two-stage translation；
- shadow page tables versus nested/stage-2 page tables；
- interrupt virtualization and device assignment boundaries；
- QEMU object/machine/bus/device/memory-region composition；
- KVM `/dev/kvm`, VM, vCPU, memory slot and run loop split；
- IOMMU device identity, translation context, queue, invalidation, fault and completion semantics；
- ATS/PASID/PRI and device-side translation-cache boundaries；
- ArchLab transaction identity and retry/completion rules across QEMU/KVM/IOMMU adapters；
- integration plan among `archlab-rv64-virt-v0`, QEMU frontend and ArchLab-virt。

完成标准：

- 六个 groups 均达到 L1 或被有证据地降级；
- 写出 `topics/virtualization.md`；
- 写出 virtualization/IOMMU boundary decision；
- 更新 A canonical inventory coverage；
- 明确哪些功能属于 RVSoC-Sim，哪些属于 ArchLab-virt，哪些通过 adapter 共享。

## 后续批次

1. PCIe / NVMe / SSD / CXL；
2. RAS / ECC / Chipkill / fault injection / recovery；
3. Memory / Cache / DRAM / NVM（剩余细分资料）；
4. Simulation & Modeling（剩余工具/论文）；
5. Linux/OS（剩余专题）；
6. Parallel / NUMA / HPC / synchronization；
7. Compiler / JIT / binary translation；
8. GPU / NPU / DPU / SIMT / collectives；
9. Power / thermal / energy；
10. Security / confidential computing；
11. Firmware / UEFI；
12. directly relevant software engineering。

## B-class policy

429 raw B records 保留且可搜索。只在里程碑、引用链或设计缺口需要时进入内容审阅，不进行表演式“全部精读”。
