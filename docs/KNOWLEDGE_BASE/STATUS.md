# Knowledge Base Status

> Last updated: 2026-08-03

这份文档是 ArchLab 资料审阅工作的接力点。它只记录已经完成到什么程度，不把“看过文件名”冒充“读过内容”。

## 1. 当前完成度

### L0：全量粗筛已完成

已纳入 **623 条 PDF 记录**：

- Books 清单：520 条；
- Paper 文件夹：103 条。

文件名粗筛结果：

- S：36 条；
- A：145 条；
- B：398 条；
- R（文件名不足，必须人工查看）：43 条；
- C（当前明显弱相关）：1 条。

初步识别出 62 条重复记录，约 31 组。该结果目前基于标题归一化，并非文件哈希或全文比对。

### L1：真实内容审阅进行中

第一批已完成 **8 个 canonical works** 的 L1 审阅：

1. Fundamentals of Full-Platform Simulation / Программное моделирование вычислительных систем；
2. Discrete-Event System Simulation；
3. System Design with SystemC；
4. Transaction Level Modeling with SystemC；
5. System Simulation with gem5, SystemC and Other Tools；
6. Software and System Development Using Virtual Platforms: Full-System Simulation with Simics；
7. SimBricks；
8. Ramulator 2.0。

详细结果见：

- `reviews/batch-01-simulation-foundation.md`

## 2. 尚未完成

**全库尚未全部读完。** 当前未完成内容包括：

- 其余 S 类 canonical works 的 L1 审阅；
- 43 条 R 类资料的目录/摘要人工确认；
- A 类资料按里程碑逐批审阅；
- 重复文件的元数据、大小、版本和内容级确认；
- 623 条清单拆分为仓库中的正式 books/papers inventory；
- 每个主题的 topic map 和 design-decision 回写。

因此，当前状态应准确表述为：

> 全量资产盘点和文件名粗筛完成；第一批核心模拟器资料完成真实内容审阅；其余资料仍按队列推进。

## 3. 当前已确认的跨资料结论

第一批资料共同支持以下方向：

1. 保持稳定的架构状态和事务语义契约；
2. functional、transaction、timing、cycle 和外部协同由不同执行/传输适配器承载；
3. Completion 应显式、可序列化，不把进程内 callback 固化为永久协议；
4. 功能正确性与性能精度必须分开声明和验证；
5. 确定性、trace、验证和 checkpoint 应尽早成为框架能力；
6. 在单进程事务协议稳定前，不急着实现跨进程同步或 PDES。

## 4. 与当前 M0.3 的关系

资料审阅支持当前同步重入规则：

```text
issue()
  ├─ send 前登记 pending
  ├─ send() 可能同步重入 on_response()
  ├─ 不允许 iterator/pointer/reference 跨过 send()
  └─ send 返回后按 TransactionKey 重新查找
```

这些结论是 M0.3C 的设计依据之一，但最终行为仍以代码、测试、`BASELINE.md` 和 `CURRENT_STATE.md` 为准。

## 5. 下一步

下一批按 `REVIEW_QUEUE.md` 推进，顺序是：

1. CPU / ISA / OOO 微架构；
2. Memory / Cache / Coherence / NoC；
3. Performance / Datacenter / Methodology；
4. Linux MM / Full-system integration；
5. R 类人工确认。
