# Knowledge Base Status

> Last updated: 2026-08-03

这份文档是 ArchLab 资料审阅工作的接力点。它只记录已经完成到什么程度，不把“看过文件名”冒充“读过内容”。

## 1. 全量粗筛

L0 已完成，共纳入 **623 条 PDF 记录**：

- Books：520 条；
- Paper：103 条；
- S：36 条；
- A：145 条；
- B：398 条；
- R（必须人工确认）：43 条；
- C（当前弱相关）：1 条。

初步识别出 62 条重复记录，约 31 组。该结果主要基于标题归一化，并非全部经过文件哈希或全文比对。

## 2. L1 真实内容审阅

目前共完成 **21 个 canonical works**。

### Batch 1 — Simulation foundation（8 项）

1. Fundamentals of Full-Platform Simulation；
2. Discrete-Event System Simulation；
3. System Design with SystemC；
4. Transaction Level Modeling with SystemC；
5. gem5 + SystemC integration；
6. Full-System Simulation with Simics；
7. SimBricks；
8. Ramulator 2.0。

记录：`reviews/batch-01-simulation-foundation.md`

### Batch 2 — CPU / ISA / OOO and SoC boundaries（4 项）

9. Modern Processor Design: Fundamentals of Superscalar Processors；
10. Computer Architecture: A Quantitative Approach, Sixth Edition；
11. Processor Microarchitecture: An Implementation Perspective；
12. Modern System-on-Chip Design on Arm。

记录：

- `reviews/batch-02-cpu-isa-ooo.md`
- `topics/cpu.md`
- `inventory/batch-02-cpu-isa-ooo.csv`

### Batch 3 — Memory / Cache / Coherence / NoC（5 项）

13. Memory Systems: Cache, DRAM, Disk；
14. The Memory System: You Can’t Avoid It, You Can’t Ignore It, You Can’t Fake It；
15. A Primer on Memory Consistency and Cache Coherence, Second Edition；
16. On-Chip Networks, Second Edition；
17. GARNET: A Detailed On-Chip Network Model inside a Full-System Simulator。

记录：

- `reviews/batch-03-memory-coherence-noc.md`
- `topics/memory.md`
- `topics/noc.md`
- `decisions/memory-coherence-noc-boundaries.md`
- `inventory/batch-03-memory-coherence-noc.csv`

### Batch 4 — Performance / Datacenter / Methodology（4 项）

18. The Art of Computer Systems Performance Analysis；
19. Performance Modeling and Design of Computer Systems；
20. The Datacenter as a Computer；
21. Computer Architecture Performance Evaluation Methods。

记录：

- `reviews/batch-04-performance-datacenter-methodology.md`
- `topics/performance.md`
- `decisions/performance-evaluation-methodology.md`
- `inventory/batch-04-performance-methodology.csv`

Batch 4 canonicalization notes：

- Raj Jain 的两份同名、同大小文件暂记为 probable duplicate；
- Harchol-Balter 的截断标题文件仍需内容/大小确认；
- *The Datacenter as a Computer* 两份不是重复：一份是 2013 第二版，一份是 2018/2019 第三版；第三版作为主版本。

## 3. 还剩多少

不能用 `623 - 21` 计算剩余，因为 623 是原始文件记录，21 是合并版本与重复后的 canonical works，两者不是同一种计数单位。

当前可准确说明：

- **43 条 R 类原始记录**必须逐个打开首页、目录或摘要才能定级；
- **145 条 A 类原始记录**需要按 ArchLab 里程碑做 L1 审阅，canonicalization 后实际作品数会少于 145；
- **398 条 B 类原始记录**已保留在知识库，但原则上按专题需要读取，不计划为了“读完”而全部精读；
- **1 条 C 类**当前不进入项目资料主线；
- 已明确排队的下一批是 Linux MM / full-system integration，之后先处理 43 条 R 类，防止遗漏被错误归类的重要资料。

因此，当前准确进度是：

> 623 条全量资产已粗筛；21 个核心 canonical works 已完成 L1；仍有 43 条 R 类待逐个确认，以及 145 条 A 类按模块逐批审阅。精确剩余 canonical-work 数量要等 R 类与重复版本继续合并后才能给出。

## 4. 尚未完成

- Batch 5：Linux MM / full-system integration；
- 43 条 R 类资料的人工确认；
- A 类资料按 Virtualization、PCIe/Storage/CXL、RAS/ECC、Parallel/NUMA、Compiler/JIT、Heterogeneous、Power/Thermal 和 Security 等里程碑逐批审阅；
- 重复文件的版本、大小、元数据和内容级确认；
- 623 条记录按领域拆分为正式 inventory；
- 已确认结论向正式设计文档和 `BASELINE.md` 的逐项回写。

## 5. 当前跨资料结论

### 5.1 模拟器框架

1. 保持稳定的架构状态和事务语义契约；
2. functional、transaction、timing、cycle 和外部协同由不同适配器承载；
3. Completion 应显式、可序列化，不把进程内 callback 固化为永久协议；
4. 功能正确性与性能精度必须分别验证；
5. 确定性、trace、验证和 checkpoint 应尽早进入框架；
6. 单进程协议稳定前，不急着实现跨进程同步或 PDES。

### 5.2 CPU / ISA / OOO

1. 架构状态与微架构状态分离；
2. functional core、timing in-order core 和 detailed OOO core 有明确边界；
3. 指令可以乱序执行/完成，但架构状态在 commit 边界最终生效；
4. 内存请求完成、指令执行完成和指令 commit 是不同事件；
5. ROB、LSQ、MSHR 等内部索引不能成为公共 Transaction 身份；
6. 性能归因按 instruction flow、register data flow 和 memory data flow 组织。

### 5.3 Memory / Coherence / NoC

1. ISA memory model 定义软件可见的合法行为；core pipeline 与 coherence protocol 共同执行该语义；
2. coherence protocol、network interface、packet/flit transport 和 DRAM backend 必须分层；
3. protocol message 不是 packet/flit，packetization 和 reassembly 属于 adapter/network interface；
4. Transport blocked、protocol `RetryLater` 和 accepted-but-pending-completion 是不同状态；
5. 固定延迟、简单排队和详细 DRAM 是不同精度模型，实验必须声明所用 backend；
6. MSHR、directory entry、DRAM queue entry、VC 和 credit 是子系统私有状态，不能替代 `TransactionKey`；
7. routing deadlock、flow-control deadlock 和 protocol deadlock 必须分别验证；
8. 研究 NoC/DRAM timing feedback 时，详细模型必须参与同一虚拟时间因果循环，预生成 trace 的结论范围要单独声明。

### 5.4 Performance / Datacenter / Methodology

1. 每个实验必须记录 research question、scope、workload、resolved config、per-component fidelity、warmup/ROI、seed/repetitions、metrics、validation target 和 exclusions；
2. raw counters/events 与 derived metrics 分层，归一化和聚合规则必须可追溯；
3. 固定延迟、排队模型、trace、execution-driven、full-system 和 hardware measurement 各有明确适用范围；
4. Little's Law 与 queueing bounds 用作 sanity check，而不是替代详细协议/设备模型；
5. workload representativeness、metric selection、baseline、averaging、confidence 和图表表达都可能造成错误结论；
6. Top-down 可视化必须保存从服务目标到 workload/VM/process/component/transaction/event 的因果和数据 provenance；
7. datacenter 是系统边界之一，不表示单节点效率不重要；应由服务瓶颈、tail、availability、power 和 cost 决定研究层级。

## 6. 与当前 M0.3 / M0.4 的关系

### M0.3

```text
issue()
  ├─ send 前登记 pending
  ├─ send() 可能同步重入 on_response()
  ├─ 不允许 iterator/pointer/reference 跨过 send()
  └─ send 返回后按 TransactionKey 重新查找
```

Transaction 不应携带 MSHR、directory、DRAM queue 或 router VC 的内部指针。

### M0.4

必须区分：

```text
Accepted
    receiver 已获得协议所有权并欠 completion

RetryLater
    receiver 未接受或保留请求

Transport blocked/backpressured
    packet/flit 暂时不能前进，但协议所有权不自动返还
```

### Future Stats / Trace

Batch 4 增加的约束：Stats、Trace、ROI、resolved config 和 run metadata 必须让每个结果能够复现并追溯其 fidelity、时间范围、工作负载和派生公式。

## 7. 下一步

下一批按 `REVIEW_QUEUE.md` 推进：

1. Batch 5 — Linux MM / full-system integration；
2. Batch 6 — 43 条 R 类人工确认；
3. 后续 A 类按里程碑进入 Virtualization、PCIe/Storage/CXL、RAS/ECC、Parallel/NUMA、Compiler/JIT、Heterogeneous、Power/Thermal 和 Security。
