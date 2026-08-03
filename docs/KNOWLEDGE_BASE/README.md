# ArchLab Knowledge Base

这套目录保存 ArchLab RVSoC-Sim 使用的书籍、论文、规范和外部模拟器资料的**版本化索引与审阅结论**。

它解决两个问题：

1. 换对话、换工作线或隔一段时间后，不再依赖聊天记忆重新猜资料用途；
2. 开发某个模块时，可以追溯“参考了哪些资料、借鉴了什么、哪些内容不能照搬”。

## 1. 存储分工

### 原始资料

原始 PDF、TXT 和配套文件继续保存在 Google Drive `books` 资料库中：

- 总目录：`https://drive.google.com/drive/folders/1V5KYlHqoF8YzgsrA8ZNsljEvVoRq8Fbc`
- Paper 目录：`https://drive.google.com/drive/folders/12go7cY-e8935_e9WkHuQVcf0K8cI256Z`
- 原始书目清单：Drive 文件 `books_pdf_list.txt`，ID `1-zWScER3SoSm6DZf8IT7JITmkEypB5mI`

GitHub 不保存大体积原始 PDF，避免仓库膨胀和版权、版本管理问题。

### 可审计知识库

GitHub 保存：

- 资料清单和去重结果；
- 分类、优先级和 ArchLab 模块映射；
- 每批真实阅读后的审阅记录；
- 从资料中形成的工程决定；
- 当前进度和下一批队列。

GitHub 是跨对话恢复时的权威入口，聊天和临时生成文件不是最终事实来源。

## 2. 审阅级别

- **L0**：只根据文件名做保守粗筛；
- **L1**：实际查看目录、摘要、前言、引言和关键架构章节；
- **L2**：精读选定章节或完整短论文；
- **L3**：提取实现接口、状态机、算法、验证方法，并写入 ArchLab 设计决定。

任何资料只有达到 L1 以上，才允许声称“已经读过内容”。

## 3. 目录结构

```text
docs/KNOWLEDGE_BASE/
├── README.md
├── STATUS.md
├── SOURCE_MANIFEST.md
├── REVIEW_QUEUE.md
└── reviews/
    └── batch-01-simulation-foundation.md
```

后续会继续增加：

```text
inventory/
    books.csv
    papers.csv
    duplicates.csv

topics/
    simulation.md
    cpu.md
    memory.md
    noc.md
    linux.md
    virtualization.md
    pcie-storage.md
    ras.md
    performance.md
    heterogeneous.md

decisions/
    transaction-completion.md
    fidelity-and-adapters.md
    validation-methodology.md
```

## 4. 新对话恢复规则

涉及资料、架构设计或参考来源时，新会话应依次读取：

1. `docs/PROJECT_MEMORY.md`
2. `docs/CURRENT_STATE.md`
3. `docs/KNOWLEDGE_BASE/STATUS.md`
4. `docs/KNOWLEDGE_BASE/REVIEW_QUEUE.md`
5. 与当前模块对应的 review/topic/decision 文档

代码和测试仍然优先于知识库；知识库用于解释来源、设计理由和后续阅读计划。

## 5. 维护原则

- 宁可把不确定资料放入人工复核，也不因文件名模糊而误删；
- 中文版、英文版、扫描版和重复下载合并成一个 canonical work，但保留原始记录；
- 资料结论必须区分“来源明确支持”“工程推断”“ArchLab 自己的决定”；
- 每完成一批审阅，更新 `STATUS.md` 和 `REVIEW_QUEUE.md`；
- 形成稳定工程契约后，将结论写入 `docs/DECISIONS.md`、专题设计文档或 `BASELINE.md`，不能只留在资料笔记里。
