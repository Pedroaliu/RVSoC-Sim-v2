# Knowledge Base Status

> Last updated: 2026-08-03

这是 ArchLab 资料工作的跨对话接力点。raw asset、canonical group、triage、L1 review 和工程决定分别记录。

## 1. 全量资产

623 条 PDF raw records：Books 520，Paper 103。

当前优先级：

```text
S:  41
A: 151
B: 429
C:   2
R:   0
Total: 623
```

当前 raw inventory：

- `inventory/normalized-snapshot-v6.md`
- Drive ID `1Pn3mmhEGkWySAmqOMpzha7D-hpDiUYJr`
- SHA-256 `d2a6767a8361985bde98176bdf4cf2feee91ea5f35957d873ed5ca59167d532c`

v6 把 Batch 10 的内容审阅状态与 canonical identity 写回 20 条 raw records，并将 4 条内容确认后的核心资料从 A 提升为 S。全部 623 条记录仍然保留。

## 2. L1 内容审阅

现有 **62 个 canonical works/source families** 达到 L1：

- Batch 1 Simulation foundation：8；
- Batch 2 CPU / ISA / OOO / SoC：4；
- Batch 3 Memory / Coherence / NoC：5；
- Batch 4 Performance / Datacenter / Methodology：4；
- Batch 5 Linux MM / Virtual Memory：4；
- Batch 5B RISC-V Linux boot contracts：6；
- Batch 8 Virtualization / QEMU / KVM / IOMMU：6；
- Batch 9 PCIe / NVMe / SSD / CXL：7；
- Batch 10 RAS / ECC / fault injection / recovery：18。

Batch 6、7 分别是 R triage 与 A canonicalization，不计入 L1 数量。

## 3. Canonical coverage

```text
A canonical groups total:       121
L1 covered:                      38 groups / 56 raw records
Remaining not-yet-L1:           83 groups / 100 raw records
```

当前 manifest：

- `inventory/a-canonical-groups-v4.md`
- Drive ID：`1vHK1N2Dw6jBUqA5NxvYl9xC5VopPMsDe`
- SHA-256：`cc971d4e1dd338e45bb658513e4af9bfcf219f914ca6f499c769c4ba955d750b`

Remaining domains：

```text
Memory / Cache / DRAM / NVM / CXL                    35
Simulation & Modeling                                11
OS / Linux / Kernel / Virtual Memory                 11
CPU / ISA / Microarchitecture                         8
Parallel / HPC / Concurrency / NUMA                   8
Performance / Datacenter / Benchmarking               3
AI / GPU / NPU / Heterogeneous                        2
Security / Trusted Computing                          2
Firmware / Boot / UEFI / BIOS                         1
NoC / Interconnect / Networking                       1
RTL / Digital Logic / FPGA                            1
```

Storage / PCIe / NVMe / SSD / USB 与 RAS / Reliability / ECC 两个 A-level domain 均已完成 L1。

## 4. Batch 10 — RAS / ECC / Fault Injection / Recovery

Status: **L1 complete**

Batch 10 审阅 18 个 canonical groups，覆盖：

- 体系结构级 fault → error → failure、masking、AVF、detection、recovery、diagnosis 与 self-repair；
- Fault-Tolerant Systems 的可靠性、可用性、MTTF/Markov、冗余、编码、ABFT 与恢复框架；
- DRAM 现场错误数据、CE/UE、hard/soft fault、scrub 和 retirement 方法；
- SECDED、BCH、Reed–Solomon、symbol-oriented Chipkill-style 与 adjacent-bit ECC；
- application/object/MPI/GPU fault injection、DVF、natural resilience、ABFT/ECC 协同与 campaign methodology；
- FPGA SEU、TMR、scrubbing、bitstream injection 与 coverage/overhead 评估。

Records：

- `reviews/batch-10-ras-ecc-reliability.md`
- `topics/ras.md`
- `decisions/ras-fault-recovery-boundaries.md`
- `inventory/batch-10-ras-ecc.csv`

Batch CSV Drive ID：`1Z_JgDQAjG0WGqrPxi58V65aSdHzG_f4R`；SHA-256 `bd33a11b5f3438e85b5cc6907584c114701619bd92478b6138ce183a06b0f289`。

## 5. Accepted RAS direction

### 5.1 Layered event model

```text
physical or injected fault
    ↓ manifestation / masking
component error state
    ↓ detection / correction
architectural RAS event
    ↓ containment / notification
firmware / OS recovery
    ↓
service / isolation / retirement / repair
```

- fault、error、failure、report 和 recovery 是不同对象；
- 注入 fault 不自动等于检测到 error，更不自动等于软件可见 failure；
- corrected error 仍可更新 CE history 并触发 scrub、threshold 或 retirement policy；
- uncorrectable error 在成功 containment 时不等于 silent corruption。

### 5.2 ECC contract

ECC 是可替换组件，显式定义：codeword layout、symbol width、check bits、encode/check/correct latency、syndrome、location information，以及 detected/corrected/uncorrectable/miscorrected outcome。

代码策略与 DIMM/rank/device/beat/symbol 映射分离。`Chipkill=true` 不是足够的模型配置。

### 5.3 Recovery and observability

Retry、corrected writeback、patrol scrub、line/page delete、page retirement、spare substitution、rank/device/core deconfiguration 与 checkpoint rollback 都是独立的定时事件。

原始 fault/syndrome/event stream 保持不可变；CE counter、adaptive threshold 和 retirement table 是可替换 policy state。

### 5.4 Fault-injection methodology

每次 campaign 必须保存 fault model、目标总体、空间/时间分布、seed、注入时刻、workload/input、ROI、重复次数、停止/收敛规则、confidence、检测点和 outcome taxonomy。

AVF、DVF、SDC rate、CE/UE incidence、MTTF/MTBF 与 availability 不得混为一个指标。

### 5.5 Product-level target

长期保持 POWER-inspired 的产品级分层目标：

```text
detect → classify → contain → recover → diagnose → isolate/retire → report/service
```

但 RAS 实现顺序仍服从当前主线：M0.3 → M0.4 → functional RISC-V/Linux。Batch 10 只建立知识与契约，不提前插入实现。

## 6. Initial RAS implementation ladder

```text
RAS-0  Deterministic fault object + one-shot injection + event log
RAS-1  SECDED component + syndrome + CE/UE outcomes
RAS-2  Scrub, CE history, thresholds and page/line retirement
RAS-3  Configurable symbol/device geometry + Chipkill-style policies
RAS-4  Core/cache/NoC/PCIe fault propagation and containment
RAS-5  Firmware/OS/service-processor recovery and deconfiguration
RAS-6  AVF/DVF/MTTF experiments and application-aware policies
```

## 7. Next batch

Batch 11：**remaining Memory / Cache / DRAM / NVM / CXL**，共 35 个 not-yet-L1 A groups。先做内部 canonical subgroup 划分，再分别处理 cache/controller、DRAM、NVM/CXL 与相关模拟器/性能资料，避免一次把 35 个作品混成一个结论。

Accurate state：

> All 623 raw assets are normalized; R=0; 62 source families have L1 review; 38 of 121 A groups are covered and 83 remain. The RAS/ECC A domain is fully reviewed at L1, with source-backed fault, ECC, injection and recovery boundaries recorded in GitHub.
