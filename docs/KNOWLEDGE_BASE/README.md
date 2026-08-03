# ArchLab Knowledge Base

这套目录保存 ArchLab RVSoC-Sim 使用的书籍、论文、规范和外部模拟器资料的**版本化索引、实际审阅与工程映射**。

## 1. 存储分工

### Google Drive：原始资产

- Books 根目录：`https://drive.google.com/drive/folders/1V5KYlHqoF8YzgsrA8ZNsljEvVoRq8Fbc`
- Paper 目录：`https://drive.google.com/drive/folders/12go7cY-e8935_e9WkHuQVcf0K8cI256Z`
- 原始书目：`books_pdf_list.txt`，ID `1-zWScER3SoSm6DZf8IT7JITmkEypB5mI`

GitHub 不保存大体积 PDF。

### GitHub：可审计知识库

保存 raw inventory、canonicalization、分类、真实审阅、主题地图、设计决定、进度和队列。GitHub 是跨对话恢复时的权威入口。

## 2. 状态层级

- **L0**：文件名粗筛；
- **Triage**：解决模糊标题和领域；
- **Canonicalized**：合并版本、翻译、复制件和 companion 的关系；
- **L1**：查看真实目录、摘要、前言、引言和关键章节；
- **L2**：精读选定章节或完整短论文；
- **L3**：提取接口、状态机、算法、验证方法并形成项目决定。

Triage/Canonicalized 不自动等于 L1。

## 3. 当前目录

```text
docs/KNOWLEDGE_BASE/
├── README.md
├── STATUS.md
├── SOURCE_MANIFEST.md
├── REVIEW_QUEUE.md
├── inventory/
│   ├── README.md
│   ├── normalized-snapshot-v2.md
│   ├── normalized-snapshot-v3.md
│   ├── a-canonical-groups-v1.md
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
│   ├── batch-06-r-class-triage.md
│   └── batch-07-a-canonicalization.md
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

## 4. 当前权威快照

### 全量 raw inventory v3

- Manifest：`inventory/normalized-snapshot-v3.md`
- Drive ID：`1Rcm-ivqexORPxfz07ThJe_-afTGES361`

### A canonical groups v1

- Manifest：`inventory/a-canonical-groups-v1.md`
- Drive ID：`1VwbkcSjDeJBZ070M-joYdt3XuEHBil6F`

旧快照保留用于审计，不能静默覆盖。

## 5. 新对话恢复顺序

1. `docs/PROJECT_MEMORY.md`
2. `docs/CURRENT_STATE.md`
3. `docs/KNOWLEDGE_BASE/STATUS.md`
4. `docs/KNOWLEDGE_BASE/REVIEW_QUEUE.md`
5. `docs/KNOWLEDGE_BASE/inventory/normalized-snapshot-v3.md`
6. `docs/KNOWLEDGE_BASE/inventory/a-canonical-groups-v1.md`
7. 当前模块对应的 review/topic/decision 文档

代码与测试优先于知识库；知识库解释来源、设计理由与后续阅读计划。

## 6. 维护原则

- 不因糟糕文件名误删资料；
- raw records 永久可追溯；
- probable duplicate 不冒充 byte-identical；
- 区分来源支持、工程推断与 ArchLab 决定；
- 权威来源缺口显式排队；
- 每批立即更新状态、队列、inventory 和 review；
- 稳定契约最终写入正式设计文档或 `BASELINE.md`。
