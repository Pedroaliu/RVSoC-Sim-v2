# Project Memory

这份文档保存 ArchLab RVSoC-Sim 的长期上下文，供开发者、评审者和 AI 助手在不同工作会话之间恢复项目目标。它只记录相对稳定的事实；当前分支和下一步见 `docs/CURRENT_STATE.md`，已实现契约见 `docs/BASELINE.md`，阶段路线见 `docs/ROADMAP.md`。

## 1. 项目定位

ArchLab RVSoC-Sim 是一个面向现代计算机体系结构、云计算和异构系统研究的，多精度、模块化、可验证、高性能全系统 RISC-V SoC 模拟器。

它不是围绕单门课程搭建的临时实验，也不是只计算 IPC 的微结构玩具。长期目标是建立一套可以运行真实软件、替换组件模型、连接外部模拟器并追溯性能问题的研究平台。

最终研究范围包括：

- 自研 RISC-V functional、timing、in-order、out-of-order、SMT 和 Vector core；
- Cache、目录一致性、AMBA CHI、NoC、DDR/HBM；
- PCIe Root Complex、Endpoint、DMA、MSI/MSI-X、IOMMU、ATS/PRI、SR-IOV、NVMe 和 CXL；
- OpenSBI、Linux、设备树、驱动、QEMU/KVM 和虚拟化；
- GPU、NPU、DPU 等异构加速器；
- 功耗、温度、RAS、故障注入、恢复和 checkpoint；
- 从业务、VM、进程和函数一路下钻到 Core、Cache、NoC、DDR、PCIe 和 cycle 的 Top-down 可视化。

## 2. 总体原则

### 2.1 局部精确，其余快速

精度属于组件，不属于整个模拟器。研究某个组件时，该组件使用详细模型，其他组件可以使用功能模型、事务模型、trace、QEMU、KVM 或外部模拟器。

例如：

- 研究 OOO core 时，设备和远端系统可以简化；
- 研究 PCIe Endpoint 时，CPU/Linux 可以由 QEMU 快速执行；
- 研究 CHI/NoC 时，指令执行可以功能化，而一致性和网络详细建模；
- 研究 CPU-GPU 一致性时，计算可以简化，共享虚拟内存和一致性详细建模。

先支持静态的 per-component fidelity；之后支持 checkpoint 边界切换；在线 ROI 动态切换放到更后面。

### 2.2 功能与时间分离

组件尽量把“状态如何改变”和“何时完成、占用哪些资源”分开。改变 latency、queue 或 contention 不应改变功能结果。

### 2.3 准确性相对于研究问题

模型没有脱离问题的绝对准确性。实验必须说明：研究问题、详细组件、简化组件、工作负载、warmup/ROI、指标、重复次数、参考模型和结论适用范围。

### 2.4 接口支持模型替换

同一个 Core、Cache、NoC、DRAM 或设备可以有多套实现。上层只依赖稳定契约，不依赖某个详细模型的内部状态。

### 2.5 平台契约先于复杂模型

在 OOO、CHI、PCIe 和 GPU 之前，先完成确定性时间、生命周期、Port/Link、Transaction/Completion、Queue/Backpressure、current/next、Stats/Trace 和 Checkpoint 等基础契约。

## 3. 原生模拟内核目标

```text
Simulation
├── deterministic EventQueue
├── SimObject lifecycle
├── typed Port / Link
├── Transaction / Completion
├── Queue / Backpressure / Retry
├── current / next state
├── ClockDomain / ResetDomain
├── Config
├── Stats
├── Trace
├── Fault hooks
└── Checkpoint / Restore
```

`Port` 和 `Link` 表达结构连接。排队、容量、延迟、信用和重试由独立的 timing/channel 组件表达，不把所有语义塞入基础 Link。

## 4. 主要技术路线

### 4.1 CPU

```text
Functional RV64 Core
→ OpenSBI + Linux
→ Timing In-order Core
→ Detailed OOO Core
→ SMT2 / SMT4
→ RISC-V Vector
```

详细 OOO 阶段最终包含 Fetch、Predictor、Decode、Rename、Physical Register File、ROB、Issue Queue、Wakeup/Select、Execution、Bypass、LSQ、Memory Disambiguation、Replay、Commit 和 Precise Recovery。

### 4.2 Cache、CHI 与 NoC

```text
Core / Cache
→ CHI RN-F protocol engine
→ CHI-NoC adapter
→ Garnet-like detailed NoC
→ CHI HN-F / SN-F
→ LLC / Memory Controller
```

CHI 负责协议、顺序、身份和事务状态；NoC 负责 packet/flit、VC、buffer、credit、router、link、arbitration、congestion 和 timing。协议重试不能由网络 VC full 直接推导。

### 4.3 PCIe、IOMMU 与 CXL

Root Complex 同时是 host config/MMIO 的 target，也是设备 DMA 访问内存系统时的 requester。路线从 ECAM、BDF、BAR、MSI/MSI-X 和简单 DMA 开始，再进入 TLP、Tag、Outstanding、Out-of-order Completion、MPS/MRRS、RTL Endpoint、credit/link timing，以及 IOMMU、PASID、ATS/PRI、SR-IOV、AER/DPC、Hotplug 和 CXL。

### 4.4 GPU 与其他加速器

路线从行为级 GPU Endpoint 开始，再连接 GPGPU-Sim/Accel-Sim 等外部详细后端，之后自研最小 SIMT Core、Warp、Mask、Scoreboard、Operand Collector、Replay、Coalescer、GPU Cache/NoC/Memory Partition，最后进入 CPU-GPU coherence、IOMMU 和 shared virtual memory。

## 5. 第一个可见系统目标

首个全系统里程碑是：

```text
Self-written Functional RV64 Core
→ OpenSBI
→ Single-core Linux
→ Initramfs
→ UART BusyBox Shell
```

最小范围包括 RV64IMA、Zicsr、Zifencei、M/S privilege、CSR、trap/interrupt、Sv39、RAM/ROM、timer、最小外部中断、UART、DTB、OpenSBI 和 Linux。初期不加入多核、OOO、PCIe、IOMMU、CHI、复杂 NoC、磁盘和网络。

## 6. 外部项目的角色

- gem5：全系统组织、SimObject、Ruby/Garnet 和研究方法参考；
- QEMU：快速功能执行、Linux 和设备语义参考；
- Spike/NEMU：RISC-V 功能参考和差分验证；
- ChampSim：trace-driven CPU/cache 研究参考；
- Garnet：详细 NoC 参考；
- Ramulator 2.0：可替换 DRAM timing backend；
- SimBricks：跨进程、分布式协同模拟边界；
- SystemC/TLM 和 QBox：外部事务级设备及 QEMU 集成参考；
- GPGPU-Sim/Accel-Sim：GPU 详细模型参考或后端；
- Verilator、FireSim 和 FPGA：RTL 与硬件加速后端；
- verilog-pcie、LitePCIe、cocotbext-pcie：PCIe RTL Endpoint 和协议验证参考。

这些项目是技术和验证来源，不要求全部同时接入，也不替代项目自己的核心契约。

## 7. 资料体系

项目的重要理论与领域资料包括：

- Software Modeling of Computing Systems：模拟器方法论、DES、功能/时间分离、多精度、PDES 和 checkpoint；
- AMBA CHI Issue H：CHI 协议权威规范；
- On-Chip Networks：NoC、VC、credit、router pipeline 和 deadlock；
- Processor Microarchitecture: An Implementation Perspective：OOO、MSHR、Rename、Issue、ROB、LSQ 和 Recovery；
- Single-Instruction Multiple-Data Execution：SIMD/Vector、Mask、Gather/Scatter 和部分完成；
- General-Purpose Graphics Processor Architectures：SIMT、Warp、Scoreboard、Operand Collector、Replay、Coalescing 和 GPU memory system；
- Memory Consistency and Cache Coherence、Virtual Memory、Hardware Prefetching、Cache Replacement、Multi-Core Cache Hierarchies、Multithreading、Performance Evaluation、Power Efficiency、FPGA Simulation、Datacenter Modeling 和 Architecture Optimization 等专题资料。

具体资料只在相关阶段进入实现，避免资料入库打断当前里程碑。

## 8. 权威来源顺序

发生冲突时按以下顺序判断：

1. 仓库当前代码与测试；
2. `docs/BASELINE.md` 中已完成的契约；
3. `docs/CURRENT_STATE.md` 中当前工作状态；
4. `docs/DECISIONS.md` 或当前里程碑记录；
5. `docs/ROADMAP.md`；
6. 本文件的长期目标；
7. 聊天记录和临时讨论。

聊天可以帮助推理，但不能替代版本化的工程事实。
