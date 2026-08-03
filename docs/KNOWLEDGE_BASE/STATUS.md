# Knowledge Base Status

> Last updated: 2026-08-03

这是 ArchLab 资料工作的跨对话接力点。它严格区分 raw asset、canonical group、triage、L1 内容审阅和工程决定。

## 1. 全量资产

已纳入 **623 条 PDF raw records**：Books 520，Paper 103。

当前 raw-record 优先级：

```text
S:  36
A: 156
B: 429
C:   2
R:   0
Total: 623
```

权威快照：

- `inventory/normalized-snapshot-v3.md`
- Drive ID：`1Rcm-ivqexORPxfz07ThJe_-afTGES361`
- SHA-256：`61388baed6db3474fefcaddb362d77647297ef404e864f0757b9159b08acd150`

`PMU_DATA.pdf` 经内容确认是电力系统励磁监测论文，PMU 指 phasor measurement unit，不是处理器性能监控单元，因此从 A 降为 C。

## 2. 已完成 L1 内容审阅

共 **31 个 canonical works/source families**：

- Batch 1 Simulation foundation：8；
- Batch 2 CPU / ISA / OOO / SoC：4；
- Batch 3 Memory / Coherence / NoC：5；
- Batch 4 Performance / Datacenter / Methodology：4；
- Batch 5 Linux MM / Virtual Memory：4；
- Batch 5B RISC-V Linux boot contracts：6。

对应 review/topic/decision 文档均保存在本目录树。

## 3. Batch 6 — R-class triage

Status: **complete**

```text
43 raw R records
→ 32 canonical groups
→ 12 raw / 11 groups promoted to A
→ 31 raw / 21 groups retained as B
→ R = 0
```

记录：

- `reviews/batch-06-r-class-triage.md`
- `inventory/batch-06-r-class-triage.csv`

## 4. Batch 7 — A-level canonicalization

Status: **complete**

```text
156 A raw records
→ 121 A canonical groups
→ 7 groups / 22 raw records already covered by prior L1 batches
→ 114 groups / 134 raw records remain not-yet-L1
```

权威 A-group 清单：

- `inventory/a-canonical-groups-v1.md`
- Drive ID：`1VwbkcSjDeJBZ070M-joYdt3XuEHBil6F`
- SHA-256：`5b89e81fff4010bbff00708ea41a4206ff3878ec4b1c8d7c111160ca0158865b`

记录：`reviews/batch-07-a-canonicalization.md`。

### 4.1 尚未审阅的 A groups 分布

```text
Memory / Cache / DRAM / NVM / CXL                    36
RAS / Reliability / ECC                              18
Simulation & Modeling                                11
OS / Linux / Kernel / Virtual Memory                 11
CPU / ISA / Microarchitecture                         8
Parallel / HPC / Concurrency / NUMA                   8
Virtualization / QEMU / KVM / IOMMU                   6
Storage / PCIe / NVMe / SSD / USB                     6
Performance / Datacenter / Benchmarking               3
AI / GPU / NPU / Heterogeneous                        2
Security / Trusted Computing                          2
Firmware / Boot / UEFI / BIOS                         1
NoC / Interconnect / Networking                       1
RTL / Digital Logic / FPGA                            1
```

### 4.2 Batch 7 的内容纠正

- `hardwareVirt.pdf` → *Hardware Support for Efficient Virtualization*；
- `microarchitecture.pdf` → Agner Fog, *The Microarchitecture of Intel, AMD, and VIA CPUs*；
- `2210.14324v1.pdf` → ChampSim 论文 *The Championship Simulator*；
- `2014_paper_microcode*.pdf` → *Security Analysis of x86 Processor Microcode*；
- 四个 `Computer_Organization_RiscV...` / `HandP_RISCV.pdf` raw files → 一个 *Computer Organization and Design: RISC-V Edition* canonical family；
- `PMU_DATA.pdf` → 电力系统论文，降为 C。

## 5. 当前已接受的项目方向

- 一个稳定的 architectural-state 与 semantic-transaction contract；
- functional/timing/cycle/external backends 通过 adapter 分离；
- Completion 显式、可序列化；
- architectural state 与 OOO/LSQ/ROB/MSHR 等私有状态分离；
- ISA memory model、coherence、NoC、DRAM timing 分层；
- 每次实验保存 resolved manifest、ROI、fidelity、seed、repetition 与 metric provenance；
- guest page table 属于模拟内存，TLB/page walker 属于模型状态；
- 第一台全系统机器是 `archlab-rv64-virt-v0`，Linux 入口为 `a0=hartid`、`a1=dtb_pa`、`satp=0`。

## 6. 下一步

Batch 8：**Virtualization / QEMU / KVM / IOMMU**，处理当前六个未审 A canonical groups，并结合已审 Simics、VM、RISC-V boot sources 提取：

- CPU virtualization 与 second-stage translation；
- QEMU machine/device/bus/memory-region 边界；
- KVM userspace/kernel split；
- DMA/IOMMU/fault/retry/ATS/PASID/PRI；
- `archlab-rv64-virt-v0`、QEMU frontend 与 ArchLab-virt 的复用边界。

准确状态：

> 623 条 raw assets 已全部归一化，R 已清零；31 个 source families 达到 L1；A 已归并成 121 groups，其中 7 已覆盖、114 尚未 L1。下一批从 6 个 Virtualization groups 开始。
