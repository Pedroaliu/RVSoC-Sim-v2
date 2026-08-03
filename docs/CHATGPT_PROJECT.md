# ChatGPT Project Workflow

这份文档定义如何在 ChatGPT 的“模拟器”Project 中使用多个对话而不丢失工程连续性。GitHub 文档是版本化事实来源，Project 中的聊天和文件负责快速恢复上下文。

## 1. 建议的 Project Instructions

把下面内容复制到 ChatGPT “模拟器”Project 的 Project instructions：

```text
这是 ArchLab RVSoC-Sim 长期工程项目。

仓库：Pedroaliu/RVSoC-Sim-v2

项目目标：构建面向现代体系结构、云计算和异构系统研究的，多精度、模块化、可验证、高性能全系统 RISC-V SoC 模拟器。最终覆盖 Functional/Timing/Cycle、Linux、OOO、SMT、Vector、Cache/CHI/Garnet/DDR、PCIe/IOMMU/CXL、GPU/NPU/DPU、Power/Thermal/RAS、Checkpoint 和 Top-down 可视化。

工作规则：
1. 不要在每个新对话中重新规划项目。
2. 回答前优先读取仓库中的 docs/PROJECT_MEMORY.md、docs/CURRENT_STATE.md、docs/BASELINE.md 和 docs/ROADMAP.md；涉及资料、架构来源或参考实现时，再读取 docs/KNOWLEDGE_BASE/STATUS.md、docs/KNOWLEDGE_BASE/REVIEW_QUEUE.md 和对应 review/topic/decision 文档。
3. 发生冲突时，以仓库当前代码和测试为最高优先级，其次是 BASELINE、CURRENT_STATE、正式 DECISIONS、KNOWLEDGE_BASE 的已审阅结论、PROJECT_MEMORY、ROADMAP，聊天记忆最低。
4. 采用“局部精确，其余快速”和功能/时间分离原则。
5. 当前里程碑没有完成前，不因为最终目标宏大而跳到 OOO、Linux、CHI、PCIe 或 GPU。
6. 用户希望自己实现代码。使用费曼学习法：先解释为什么，让用户预测或复述，再做最小实验、观察结果、补齐原理和写测试。一次推进一个小问题，不直接倾倒完整实现。
7. 当用户明确要求检查、修复、提交或创建 PR 时，可以操作 GitHub；普通学习阶段以指导用户自己写为主。
8. 每个工作阶段结束时，用统一交接格式总结，并更新 docs/CURRENT_STATE.md；稳定的长期决定更新 PROJECT_MEMORY、KNOWLEDGE_BASE 或专门设计文档。
```

## 2. 对话划分

同一个 Project 中使用多个专题对话，不把全部工作挤进一个无限增长的聊天。

建议保留以下工作线：

```text
00-项目总控台
    项目范围、跨模块设计决定、路线和阶段验收

01-资料入库
    上传书籍、论文、规范和模拟器代码；登记资料用途

M0.3-Transaction-Completion
    当前基础事务契约学习和实现

M0.4-Queue-Backpressure-Retry
    队列、容量、流控、重试和时序 channel

Linux-v0
    Functional RV64、OpenSBI、Sv39、UART、Timer 和 Linux 启动

Core-OOO-SMT-Vector
    Timing core、OOO、SMT 和 RISC-V Vector

CHI-Garnet-NoC
    一致性协议、Home/Directory 和详细 NoC

PCIe-IOMMU-CXL
    Root Complex、Endpoint、DMA、IOMMU 和 CXL

GPU-NPU-Heterogeneous
    SIMT、加速器和 CPU-GPU 共享系统

Validation-Performance-RAS
    Diff-test、实验方法、统计、Checkpoint、故障和恢复
```

只在相关阶段启用对应工作线。当前主要活动对话应是：

- `00-项目总控台`；
- `01-资料入库`；
- `M0.3-Transaction-Completion`。

## 3. 新对话启动语

新建专题对话时发送：

```text
请先读取本项目 GitHub 仓库中的：
- docs/PROJECT_MEMORY.md
- docs/CURRENT_STATE.md
- docs/BASELINE.md
- docs/ROADMAP.md

涉及资料或设计来源时，还要读取：
- docs/KNOWLEDGE_BASE/STATUS.md
- docs/KNOWLEDGE_BASE/REVIEW_QUEUE.md
- 当前主题对应的 review/topic/decision 文档

然后继续“<工作线名称>”。不要重新规划整个项目；以 CURRENT_STATE 中的下一步为起点。采用费曼方式带着我自己实现，一次推进一个问题。
```

若 GitHub 内容在当前会话不可访问，则把 `docs/CURRENT_STATE.md` 与 `docs/KNOWLEDGE_BASE/STATUS.md` 上传到该对话，或粘贴其中“当前工作线、已确认设计、下一步准确动作、资料审阅进度”相关章节。

## 4. 阶段交接格式

每次准备换对话、阶段结束或长时间暂停时，生成以下交接块：

```text
工作线：
日期：
仓库：
当前分支：
当前提交：
当前里程碑：

本次完成：
- 

确认的设计决定：
- 

修改文件：
- 

测试结果：
- 

仍未解决：
- 

下一步准确动作：
1. 

对其他工作线的影响：
- 
```

随后执行：

1. 当前进度写回 `docs/CURRENT_STATE.md`；
2. 稳定的长期原则写入 `docs/PROJECT_MEMORY.md` 或专门设计文档；
3. 已完成行为更新 `docs/BASELINE.md`；
4. 路线变化才修改 `docs/ROADMAP.md`；
5. 资料扫描、审阅进度和来源映射写回 `docs/KNOWLEDGE_BASE/`；
6. 代码和文档提交到 GitHub；
7. 需要时把交接消息保存为 Project source。

## 5. 信息流

```text
专题对话
    │ 产生推理、实验和决定
    ▼
GitHub code + tests
    │ 形成可执行事实
    ▼
BASELINE / CURRENT_STATE / PROJECT_MEMORY / ROADMAP
    │ 工程状态与长期方向
    ▼
KNOWLEDGE_BASE
    │ 保存资料索引、真实审阅、来源映射和不能照搬的边界
    ▼
其他专题对话继续工作
```

Project memory 可以帮助不同对话互相引用，但不把它视为严格、完整、可审计的数据库。关键工程事实和资料结论必须写回 GitHub。

## 6. 文档职责

- `PROJECT_MEMORY.md`：长期目标、总原则、总体架构和资料体系；
- `CURRENT_STATE.md`：当前分支、里程碑、已确认设计、未完成内容和下一步；
- `BASELINE.md`：已经由代码和测试保证的行为；
- `ROADMAP.md`：阶段顺序和完成标准；
- `KNOWLEDGE_BASE/STATUS.md`：资料扫描与真实审阅进度；
- `KNOWLEDGE_BASE/REVIEW_QUEUE.md`：下一批精读顺序；
- `KNOWLEDGE_BASE/reviews/`：基于真实内容的逐批审阅结果；
- 专题设计文档：CHI、PCIe、GPU、OOO 等阶段的详细设计；
- 聊天交接摘要：临时恢复工具，不作为最终权威来源。

## 7. 维护节奏

不要求每条聊天消息都更新文档。以下时点必须更新：

- 一个里程碑或子任务完成；
- 合并 PR；
- 当前分支改变；
- 一个会影响其他模块的设计决定确认；
- 一批资料完成 L1/L2/L3 审阅；
- 发现现有文档与代码冲突；
- 切换到新的专题对话；
- 项目暂停超过数天。
