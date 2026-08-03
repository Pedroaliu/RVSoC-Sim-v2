# Knowledge Base Status

> Last updated: 2026-08-03

这是 ArchLab 资料审阅工作的跨对话接力点。它区分资产盘点、文件名分类、内容审阅和工程决定，不把“找到文件”冒充“读完资料”。

## 1. 全量资产与归一化清单

已纳入 **623 条 PDF 原始记录**：

- Books：520；
- Paper：103。

Batch 6 完成后，原始记录优先级为：

```text
S:  36
A: 157
B: 429
C:   1
R:   0
Total: 623
```

原先的 43 条 R 已全部处理：12 条提升到 A，31 条归入 B，没有提升到 S，也没有排除为 C。

权威归一化快照：

- `inventory/normalized-snapshot-v2.md`
- Drive CSV ID：`1VB6a64pttPvORjXf2Tq0ENDgoNlUi4Ou`
- SHA-256：`9bff0c5af46214e949637b5df7f17eee473c5935ee6d3983b84a7d380ba121fe`

注意：以上是 raw-record 数量，不是去重后的 canonical-work 数量。

## 2. L1 真实内容审阅

目前有 **31 个 canonical works/source families** 达到 L1。

### Batch 1 — Simulation foundation（8）

- Fundamentals of Full-Platform Simulation；
- Discrete-Event System Simulation；
- System Design with SystemC；
- Transaction Level Modeling with SystemC；
- gem5 + SystemC integration；
- Full-System Simulation with Simics；
- SimBricks；
- Ramulator 2.0。

### Batch 2 — CPU / ISA / OOO / SoC boundaries（4）

- Modern Processor Design；
- Computer Architecture: A Quantitative Approach, Sixth Edition；
- Processor Microarchitecture；
- Modern System-on-Chip Design on Arm。

### Batch 3 — Memory / Coherence / NoC（5）

- Memory Systems: Cache, DRAM, Disk；
- The Memory System；
- A Primer on Memory Consistency and Cache Coherence, Second Edition；
- On-Chip Networks, Second Edition；
- GARNET。

### Batch 4 — Performance / Datacenter / Methodology（4）

- The Art of Computer Systems Performance Analysis；
- Performance Modeling and Design of Computer Systems；
- The Datacenter as a Computer；
- Computer Architecture Performance Evaluation Methods。

### Batch 5 — Linux MM / Virtual Memory（4）

- The Linux Memory Manager；
- Operating Systems: Three Easy Pieces；
- Architectural and Operating System Support for Virtual Memory；
- Local and Remote Memory in a Linux/NUMA System。

### Batch 5B — RISC-V Linux boot contracts（6）

- RISC-V Privileged Architecture `v20260120`；
- RISC-V SBI v3.0；
- Linux RISC-V Kernel Boot Requirements；
- OpenSBI platform requirements and firmware model；
- Devicetree Specification；
- upstream QEMU RISC-V `virt` platform documentation/source。

## 3. Batch 6 — R-class triage 完成

43 条模糊记录已经归并为 **32 个 canonical groups**：

- A：12 raw / 11 canonical groups；
- B：31 raw / 21 canonical groups；
- R：0。

记录：

- `reviews/batch-06-r-class-triage.md`；
- `inventory/batch-06-r-class-triage.csv`；
- `inventory/normalized-snapshot-v2.md`。

发现的关键纠正包括：

- `understand.pdf` 实际是 Mel Gorman 的 *Understanding the Linux Virtual Memory Manager*；
- `Essential Computer Science A Programmer s Guide to Foundatio.pdf` 实际是 Douglas Comer 的 *Essentials of Computer Architecture, Second Edition*；
- `wlmod.pdf` 实际是 Feitelson 的 *Workload Modeling for Computer Systems Performance Evaluation*；
- 四个模糊中文文件属于 Visualize-ML / 鸢尾花书系列；
- 七个 `section_x.xx.pdf` 是同一计算机组成教材的历史/进一步阅读补充；
- 三个数字化论文文件名已经解析为 ESTEEM、GPU 数据隐私和 WriteSmoothing 论文。

Batch 6 是分类清零，不等于这些 32 个作品全部达到 L1。只有明确标注“首页/目录/摘要已检查”的条目具有内容级 triage 证据。

## 4. 已确认的工程方向

### 模拟器框架

- 稳定的架构状态与事务语义契约；
- fidelity 和 transport 放在 adapter 后面；
- Completion 显式且可序列化；
- 确定性、trace、验证和 checkpoint 进入框架能力；
- 单进程协议正确后再做 PDES/分布式执行。

### CPU / OOO

- 架构状态与微架构状态分离；
- functional、timing in-order、detailed OOO 边界明确；
- execution completion、memory completion、commit 是不同事件；
- ROB/LSQ/MSHR 索引不是公共 transaction identity。

### Memory / Coherence / NoC

- ISA memory model、core ordering、coherence、NoC transport、DRAM timing 分层；
- protocol message 不是 packet/flit；
- transport backpressure 不是 protocol `RetryLater`；
- fixed-latency、queued、detailed memory backend 的精度声明分开；
- MSHR/directory/controller/router 私有状态不能替代 `TransactionKey`。

### Performance

- 每次运行保存 resolved experiment manifest；
- raw events、derived metrics、conclusions 分层；
- workload、ROI、seed、repetition、normalization、fidelity、exclusions 都是结果元数据；
- 分析模型用于 sanity check，不替代详细状态模型；
- Top-down 视图保存因果归因与数据 provenance。

### Linux / VM / RISC-V platform

- VMA validity、physical allocation、PTE presence 是不同状态；
- guest page tables 属于模拟内存，TLB/walker 属于模型状态；
- Linux 在 guest 内执行 allocation、CoW、reclaim、migration、OOM 等策略；
- 第一台全系统机器定义为 `archlab-rv64-virt-v0`；
- Linux-v0 基线为单 hart、RV64 M/S/U、Sv39、OpenSBI、DRAM `0x80000000`、timer/software interrupt、PLIC、NS16550 UART 和 DTB；
- Linux 入口状态：`a0=hartid`、`a1=dtb_pa`、`satp=0`。

## 5. 当前剩余工作

R 已经清零。下一阶段不再使用原先“145 条 A”作为剩余量，因为其中已有一部分被前六批吸收。

接下来需要：

1. 根据归一化快照，把 A raw records 合并成 remaining canonical groups；
2. 形成准确的未审 A 清单；
3. 按里程碑进行 L1/L2/L3：
   - Virtualization / QEMU / KVM / IOMMU；
   - PCIe / NVMe / SSD / CXL；
   - RAS / ECC / Chipkill / fault injection / recovery；
   - Parallel / NUMA / HPC / synchronization；
   - Compiler / assembly / JIT / binary translation；
   - GPU / NPU / DPU / SIMT / collectives；
   - Power / thermal / energy；
   - Security / confidential computing；
   - Firmware / boot / UEFI；
   - directly relevant C++/software-engineering sources。

398 条 B 继续保留和可搜索，按里程碑或引用链读取，不做没有目的的逐本精读。

准确状态：

> 623 条资产完成全量粗筛和归一化；31 个核心 source families 完成 L1；43 条 R 已全部清零并归并为 32 个 canonical groups；下一步是重新计算尚未审阅的 A-level canonical groups，然后按 ArchLab 里程碑逐批深入。
