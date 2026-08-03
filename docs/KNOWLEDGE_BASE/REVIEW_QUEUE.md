# Knowledge Review Queue

> Last updated: 2026-08-03

审阅顺序服从 ArchLab 里程碑。每批完成后立即写入仓库，不依赖聊天记忆积累结果。

## 已完成 L1 批次

- Batch 1 — Simulation foundation：8 canonical works；
- Batch 2 — CPU / ISA / OOO / SoC boundaries：4；
- Batch 3 — Memory / Cache / Coherence / NoC：5；
- Batch 4 — Performance / Datacenter / Methodology：4；
- Batch 5 — Linux MM / Virtual Memory：4；
- Batch 5B — RISC-V Linux boot contracts：6 source families。

L1 合计：**31 canonical works/source families**。

## Batch 6 — R-class manual triage

Status: **complete**

```text
43 raw R records
→ 32 canonical groups
→ 12 raw / 11 canonical groups promoted to A
→ 31 raw / 21 canonical groups retained as B
→ 0 remaining R
```

记录：

- `reviews/batch-06-r-class-triage.md`；
- `inventory/batch-06-r-class-triage.csv`；
- `inventory/normalized-snapshot-v2.md`。

R triage 是分类与 canonicalization，不表示全部条目达到 L1。

## Batch 7 — Recalculate remaining A canonical groups

Status: **next**

原始粗筛共有 145 条 A，Batch 6 又提升了 12 条 raw records，但前六个 L1 批次已经吸收了其中一部分。因此必须从归一化快照重新计算，不能使用 `157 - 31` 之类的错误减法。

步骤：

1. 读取 `archlab_all_records_normalized_v2.csv`；
2. 对 A raw records 做标题归一化、版本/翻译/伴随文件关系合并；
3. 标记已被 Batch 1–5B 覆盖的 canonical works；
4. 输出真正尚未达到 L1 的 A canonical groups；
5. 为每组记录主版本、Drive ID、ArchLab 模块和目标审阅阶段。

产物：

- `inventory/a-canonical-groups-v1.csv`；
- `reviews/batch-07-a-canonicalization.md`；
- 更新后的 `STATUS.md`。

## Batch 8 — Virtualization / QEMU / KVM / IOMMU

Status: **queued after Batch 7**

优先原因：它直接连接当前 functional platform、QEMU frontend、ArchLab-virt、H-extension、设备直通和 IOMMU 性能实验。

预期来源族包括：

- 系统虚拟化原理与实现；
- Hardware and Software Support for Virtualization；
- KVM 实战与原理；
- QEMU machine/device modeling；
- AMD IOMMU；
- RISC-V IOMMU；
- VT-x / ARM virtualization 支撑资料；
- Simics/virtual-platform 已审结论的交叉引用。

提取目标：

- CPU virtualization、second-stage translation、interrupt virtualization；
- QEMU machine/device/bus/memory-region 边界；
- KVM userspace/kernel split；
- IOMMU、DMA、ATS/PASID/PRI 与 fault/retry；
- functional frontend 与 timing SoC backend 的适配边界；
- ArchLab-rv64-virt-v0 与 ArchLab-virt 的复用关系。

## 后续 A-level 里程碑组

1. PCIe / NVMe / SSD / CXL；
2. RAS / ECC / Chipkill / fault injection / recovery；
3. Parallel / NUMA / HPC / synchronization；
4. Compiler / assembly / JIT / binary translation；
5. GPU / NPU / DPU / SIMT / collectives；
6. Power / thermal / energy；
7. Security / confidential computing；
8. Firmware / boot / UEFI；
9. directly relevant C++ and software-engineering sources。

## B-class references

429 raw B records are保留且可搜索。它们只在以下情形进入精读：

- 当前里程碑需要；
- S/A 来源的引用链指出它；
- 设计问题暴露出知识缺口；
- 需要构造有代表性的 workload 或验证 case。

## Immediate work sequence

```text
A raw records
    ↓ canonicalization / edition / duplicate grouping
remaining A canonical groups
    ↓
Virtualization batch
    ↓
PCIe/Storage/CXL and RAS batches
    ↓
other milestone-driven reviews
```
