# ArchLab Knowledge Base

这套目录保存 ArchLab RVSoC-Sim 使用的书籍、论文、规范和外部模拟器资料的**版本化索引与审阅结论**。

它解决两个问题：

1. 换对话、换工作线或隔一段时间后，不再依赖聊天记忆重新猜资料用途；
2. 开发某个模块时，可以追溯“参考了哪些资料、借鉴了什么、哪些内容不能照搬”。

## 1. 存储分工

### 原始资料

原始 PDF、TXT 和配套文件保存在 Google Drive `books` 资料库：

- 总目录：`https://drive.google.com/drive/folders/1V5KYlHqoF8YzgsrA8ZNsljEvVoRq8Fbc`
- Paper 目录：`https://drive.google.com/drive/folders/12go7cY-e8935_e9WkHuQVcf0K8cI256Z`
- 原始书目清单：`books_pdf_list.txt`，Drive ID `1-zWScER3SoSm6DZf8IT7JITmkEypB5mI`

GitHub 不保存大体积原始 PDF；它保存可审计的索引、审阅和设计映射。

### 可审计知识库

GitHub 保存：

- raw inventory 与 canonicalization 关系；
- 分类、优先级和 ArchLab 模块映射；
- 每批真实阅读后的审阅记录；
- 主题地图和工程决定；
- 当前进度、来源缺口和下一批队列。

GitHub 是跨对话恢复时的权威入口，聊天和临时生成文件不是最终事实来源。

## 2. 审阅级别

- **L0**：只根据文件名做保守粗筛；
- **Triage**：为模糊文件确认标题/领域，必要时查看首页、目录或摘要；
- **L1**：实际查看目录、摘要、前言、引言和关键架构章节；
- **L2**：精读选定章节或完整短论文；
- **L3**：提取接口、状态机、算法、验证方法并形成项目决定。

Triage 不能自动算作 L1。任何资料只有明确记录达到 L1，才允许声称“已经读过内容”。

## 3. 当前目录结构

```text
docs/KNOWLEDGE_BASE/
├── README.md
├── STATUS.md
├── SOURCE_MANIFEST.md
├── REVIEW_QUEUE.md
├── inventory/
│   ├── README.md
│   ├── normalized-snapshot-v2.md
│   ├── batch-02-cpu-isa-ooo.csv
│   ├── batch-03-memory-coherence-noc.csv
│   ├── batch-04-performance-methodology.csv
│   ├── batch-05-linux-mm-vm.csv
│   ├── batch-05b-riscv-linux-boot.csv
│   └── batch-06-r-class-triage.csv
├── reviews/
│   ├── batch-01-simulation-foundation.md
│   ├── batch-02-cpu-isa-ooo.md
│   ├── batch-03-memory-coherence-noc.md
│   ├── batch-04-performance-datacenter-methodology.md
│   ├── batch-05-linux-mm-virtual-memory.md
│   ├── batch-05b-riscv-linux-boot-contracts.md
│   └── batch-06-r-class-triage.md
├── topics/
│   ├── cpu.md
│   ├── memory.md
│   ├── noc.md
│   ├── performance.md
│   ├── linux.md
│   └── riscv-linux-platform.md
└── decisions/
    ├── memory-coherence-noc-boundaries.md
    ├── performance-evaluation-methodology.md
    ├── linux-vm-full-system-boundaries.md
    └── riscv-linux-machine-v0.md
```

## 4. 归一化资产快照

当前 623 条 raw records 的权威快照登记在：

- `inventory/normalized-snapshot-v2.md`
- Drive CSV ID：`1VB6a64pttPvORjXf2Tq0ENDgoNlUi4Ou`

未来更新必须生成新版本和新校验值，不静默覆盖旧快照。

## 5. 新对话恢复规则

涉及资料、架构设计或参考来源时，依次读取：

1. `docs/PROJECT_MEMORY.md`
2. `docs/CURRENT_STATE.md`
3. `docs/KNOWLEDGE_BASE/STATUS.md`
4. `docs/KNOWLEDGE_BASE/REVIEW_QUEUE.md`
5. `docs/KNOWLEDGE_BASE/inventory/normalized-snapshot-v2.md`
6. 当前模块对应的 review/topic/decision 文档

代码和测试仍优先于知识库；知识库解释来源、设计理由和后续阅读计划。

## 6. 维护原则

- 宁可将不确定资料列入人工复核，也不因文件名差而误删；
- 中文版、英文版、扫描版和重复下载合并为 canonical work，但保留全部 raw records；
- 区分“来源明确支持”“工程推断”“ArchLab 自己的决定”；
- 发现权威来源缺口时显式排队，不靠聊天记忆补全；
- 每完成一批，立即更新 `STATUS.md`、`REVIEW_QUEUE.md`、review 和 inventory/topic/decision；
- 稳定工程契约最终写入正式设计文档、`DECISIONS.md` 或 `BASELINE.md`，不能只停留在资料笔记中。
