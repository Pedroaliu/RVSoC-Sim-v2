# Architecture

## 1. 定位

ArchLab RVSoC-Sim v2 是一个执行驱动、组件化、多精度、可验证、可协同的全系统 SoC 模拟平台。它要同时服务于三类问题：

1. **体系结构研究**：CPU、Cache、Coherence、NoC、DRAM、PCIe、IOMMU；
2. **系统与虚拟化研究**：Linux、KVM/QEMU、H-extension、I/O 虚拟化；
3. **性能诊断研究**：把真实服务器观测到的队列、带宽、延迟和拓扑映射到可解释模型。

## 2. 总体结构

```text
Experiment / Control Plane
        │
        ▼
Board / Component Graph
        │
        ▼
Simulation Core
(Time, Event, Clock, Lifecycle, State, Checkpoint, Fidelity)
        │
        ├──────── Execution Engines
        │         Functional / Trace / QEMU / KVM / RTL
        │
        ├──────── Timing Models
        │         Simple / Interval / OOO / Cycle-accurate
        │
        └──────── SoC Components
                  Cache / Coherence / NoC / DRAM / PCIe / IOMMU / Devices
        │
        ▼
Verify / Observe / Performance Model
(Difftest, invariants, trace, top-down, queueing model)
```

核心不是方框数量，而是执行引擎、时序模型和外部模拟器都必须遵守同一套平台契约。

## 3. 六项核心契约

### 3.1 Time Contract

全局时间由以下顺序决定：

```text
tick → phase → delta → sequence
```

- `tick`：全局模拟时间单位；
- `phase`：同一 tick 内不同语义阶段；
- `delta`：同一阶段内零时间传播；
- `sequence`：同时间事件的稳定插入顺序。

全局 `Tick` 不等于 CPU cycle。每个组件通过：

```text
edge(n) = offset + n × period
```

把本地 cycle 映射到全局 Tick。`period` 必须大于零。

### 3.2 Lifecycle Contract

未来所有正式组件遵循：

```text
construct → configure → connect → validate → initialize → reset
          → run → drain → serialize/restore → shutdown
```

生命周期的价值在于：

- 连接完成前禁止运行；
- checkpoint 或模型切换前可以 drain；
- reset、restore 和 shutdown 行为可统一测试；
- 组件不会通过构造函数偷偷启动事件。

当前基线还没有加入 `SimObject`，它是下一阶段第一个实验。

### 3.3 Transaction Contract

组件之间通过 typed transaction 通信，而不是直接修改对方内部状态。

第一版目标：

```text
MemRequest
├── request_id
├── source_id
├── address
├── size
├── command
├── byte_enable
├── attributes
├── ordering
├── context
└── trace_id
```

接口必须最终表达：accepted、backpressure、retry、request/response 分离、outstanding request 和 ordering。否则 MSHR、NoC VC、PCIe credit 与 IOMMU queue 都会在后期迫使接口返工。

### 3.4 State Contract

状态分为三类：

- **Architectural State**：寄存器、PC、CSR、特权级、架构可见设备状态和内存；
- **Microarchitectural State**：Cache、TLB、预测器、ROB、MSHR、NoC buffer、DRAM row state；
- **External/Nondeterministic State**：外部中断、网络输入、wall clock、随机数、外部模拟器响应。

架构状态必须跨模型迁移；微架构状态可以 preserve、warm 或 flush；外部不确定状态必须记录才能 replay。

### 3.5 Commit Contract

所有 CPU backend 最终形成统一提交记录：

```text
CommitRecord
├── hart_id
├── sequence
├── pc / next_pc
├── instruction_bits
├── privilege
├── register_writes
├── csr_writes
├── memory_effects
├── exception
└── interrupt
```

它是 NEMU/Spike difftest、自研 OOO 检查、RTL 验证、trace 和 checkpoint 一致性的共同边界。

### 3.6 Observation Contract

未来每笔行为携带：

```text
timestamp
component_id
trace_id
parent_trace_id
event_type
```

这样一笔业务请求才能被追踪为：

```text
application → syscall → load/store → cache miss → coherence
            → NoC → memory queue → DRAM → response
```

这是系统 Top-down 鸟瞰图与 cycle zoom 的数据基础。

## 4. 功能与时序组合

平台将支持三条路径：

1. **Timing-first + Commit Check**：适合 OOO、错误路径、多核同步与 RTL；
2. **Functional-first + Timing Feedback**：适合快速 timing 和设计空间探索；
3. **Trace-driven**：适合单模块回放、回归、NoC/DRAM 扫描与 synthetic traffic。

详细多核模型不能依赖固定 trace 作为唯一真值，因为微架构变化可能改变线程交错和数据流。

## 5. 多精度切换

精度是每个组件的属性：

```text
Functional
LooselyTimed
ApproximatelyTimed
CycleAccurate
RTL
External
```

切换流程必须是：

```text
Trigger
→ stop admission
→ drain
→ quiescent point
→ export architectural state
→ preserve/warm/flush micro state
→ replace model
→ import state
→ warmup
→ resume
```

近期只实现 Functional → SimpleTiming，并采用 architectural state preserve 与 micro state flush/warmup。

## 6. SoC 分层

### CPU

```text
ISA Decoder
ISA Semantics
Architectural State
Execution Engine
Timing Model
Commit Interface
```

### Cache / Coherence / NoC

```text
Cache storage
Replacement/indexing
Coherence protocol
Network interface
NoC topology/routing
```

### I/O

```text
CPU / DMA
→ AddressSpace
→ IOMMU
→ PCIe Root Complex
→ Link / Switch
→ Endpoint
```

从第一版事务开始预留 Requester ID、PASID/Stream ID、VMID、ordering、traffic class 和 error 属性。

## 7. 性能模型平面

Cycle simulator 之外还将建立独立性能建模层：

- Operational model：吞吐、利用率、驻留时间、队列长度的守恒关系；
- Layered queueing model：应用、内核、驱动、设备的同步调用与资源竞争；
- Mechanistic/gray-box model：interval CPU、NoC、DRAM、PCIe credit、MLP 模型。

它不取代详细模拟，而是用于 sanity check、快速预测、参数拟合和真实服务器结果解释。
