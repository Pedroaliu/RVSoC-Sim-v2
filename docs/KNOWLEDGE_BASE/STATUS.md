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

目前共完成 **12 个 canonical works**。

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

Batch 2 修正了一项粗分类：`ModernSoC_textbook.pdf` 实际是 David J. Greaves 的 *Modern System-on-Chip Design on Arm*，主分类应是 SoC Integration / Interconnect / ESL，而不是单纯 CPU / OOO。

## 3. 尚未完成

全库尚未全部读完。仍需完成：

- 其余 S 类 canonical works 的 L1 审阅；
- 43 条 R 类资料的目录/摘要人工确认；
- A 类资料按里程碑逐批审阅；
- 重复文件的版本、大小、元数据和内容级确认；
- 623 条记录按领域拆分为正式 inventory；
- Memory、NoC、Linux、Virtualization、PCIe、RAS、Performance、Heterogeneous 等主题映射；
- 已确认结论向正式设计文档和 `BASELINE.md` 的逐项回写。

由于多个原始记录会合并成一个 canonical work，目前不公布“剩余 S 类书籍数量”的精确值，等 canonicalization 继续完成后再统计。

准确状态是：

> 全量资产盘点和文件名粗筛完成；12 个核心 canonical works 完成 L1 真实内容审阅；其余资料按批次推进，每批完成后立即写回 GitHub。

## 4. 当前跨资料结论

### 模拟器框架

1. 保持稳定的架构状态和事务语义契约；
2. functional、transaction、timing、cycle 和外部协同由不同适配器承载；
3. Completion 应显式、可序列化，不把进程内 callback 固化为永久协议；
4. 功能正确性与性能精度必须分别验证；
5. 确定性、trace、验证和 checkpoint 应尽早进入框架；
6. 单进程协议稳定前，不急着实现跨进程同步或 PDES。

### CPU 模型边界

1. 架构状态与微架构状态必须分离；
2. Functional core 只承诺软件可见语义、精确 trap 和平台交互；
3. Timing in-order core 增加 stage、hazard、current/next、FU 和 memory timing；
4. OOO core 才引入 Rename、Free List、ROB、Issue Queue、LSQ、Bypass、Commit 和 Recovery；
5. 乱序执行可以乱序完成，但架构状态必须在 commit 边界按序更新；
6. Completion 不等于 Commit：内存响应、执行完成和退休是不同事件；
7. SMT 是成熟 core 上的资源共享与调度；
8. 指标应按 instruction flow、register data flow 和 memory data flow 分层归因。

### SoC 组合

1. CPU、DMA 等 initiator 通过稳定 request/response 事务与 target 通信；
2. ROB、LSQ、MSHR 等内部结构不能成为公共互连协议；
3. Port/Link 表达连接，queue、latency、credit、ordering 和 contention 由 channel/interconnect 表达；
4. 不同抽象层通过 transactor/adapter 组合；
5. resolved configuration、statistics、assertions 和 design-space exploration 是长期框架能力。

## 5. 与当前 M0.3 的关系

当前同步重入规则不变：

```text
issue()
  ├─ send 前登记 pending
  ├─ send() 可能同步重入 on_response()
  ├─ 不允许 iterator/pointer/reference 跨过 send()
  └─ send 返回后按 TransactionKey 重新查找
```

Batch 2 进一步说明：通用事务层不能依赖未来 ROB、LSQ 或 MSHR 的内部指针。详细 core 会有多个独立生命周期，而稳定 transaction key 和 exact-once completion 必须先成立。

最终行为仍以代码、测试、`BASELINE.md`、`CURRENT_STATE.md` 和正式设计决定为准。

## 6. 下一步

下一批是 **Batch 3 — Memory / Cache / Coherence / NoC**：

1. Memory Systems: Cache, DRAM, Disk；
2. The Memory System；
3. A Primer on Memory Consistency and Cache Coherence, 2nd ed.；
4. On-Chip Networks, 2nd ed.；
5. GARNET paper。

目标是提取 request ownership、cache/MSHR/writeback、coherence state machine、consistency、NoC packet/flit/VC/credit/deadlock，以及 DRAM backend 边界。
