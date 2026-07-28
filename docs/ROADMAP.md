# Roadmap

路线图按“平台契约先于复杂模型”的顺序推进。每个里程碑都必须有可运行 demo、单元测试和完成标准。

## M0 — Core Contracts

当前已完成：

- 时间、TimePoint、TimeStamp；
- ClockDomain；
- EventQueue；
- MemoryMap / MMIO / RAM；
- Magic / ROI / Stats / FakeCpu。

下一步由学习实验完成：

1. 最小 `SimObject` 生命周期；
2. typed `Port` 与 `Link`；
3. `MemRequest` / `MemResponse`；
4. accepted / retry / backpressure；
5. pending transaction；
6. `ArchitecturalState`；
7. `CommitRecord`。

完成标准：已有基线行为不变，组件不再通过直接函数调用修改彼此私有状态。

## M1 — Minimal Functional Platform

- 最小 RV64 指令子集；
- decoder 与 ISA semantics 分离；
- ELF loader；
- RAM / ROM / MMIO AddressSpace；
- UART、Timer、Interrupt line；
- commit stream。

完成标准：运行 bare-metal 程序，UART 输出、timer interrupt 与 commit trace 可重复。

## M2 — Verification and State

- Spike/NEMU adapter；
- commit-level difftest；
- ArchitecturalState export/import；
- sparse memory；
- serialize/restore；
- pending event checkpoint；
- nondeterministic input log。

完成标准：checkpoint 前后 commit stream 连续一致，功能模型与参考模型一致。

## M3 — Timing Infrastructure

- request/response channel；
- latency、queue、capacity；
- backpressure、retry；
- SimpleTiming CPU；
- SimpleCache；
- SimpleMemoryController；
- queue occupancy 与 latency histogram。

完成标准：改变队列容量和延迟只改变性能，不改变功能结果，并能解释每段时间消耗。

## M4 — Multi-Fidelity

- ModelVariant；
- FidelityManager；
- ROI/PC/time/event trigger；
- drain 与 quiescent point；
- Functional ↔ SimpleTiming；
- warmup；
- checkpoint-assisted sampling。

完成标准：切换不丢失 outstanding request，架构状态连续，warmup 误差可量化。

## M5 — Multicore, Cache, Coherence, NoC

- private/shared cache；
- MSHR；
- directory；
- transient coherence states；
- request/response/snoop；
- router/link/VC/credit；
- protocol tester；
- memory consistency litmus tests。

完成标准：随机一致性测试、litmus、deadlock 检测和参考模型交叉验证通过。

## M6 — Full SoC and Virtualization

- MMU/TLB；
- PLIC/AIA；
- RISC-V H-extension；
- PCIe Root Complex；
- MSI/MSI-X；
- IOMMU；
- DMA；
- VirtIO/NVMe；
- 与 ArchLab-Virt 对接。

完成标准：Linux 多核启动，虚拟中断与 DMA 经 IOMMU 工作。

## M7 — Advanced Execution and Co-simulation

- QEMU TCG backend；
- KVM fast-forward；
- timing-first OOO；
- SystemC/TLM bridge；
- Verilator RTL；
- remote co-simulation protocol；
- external state/checkpoint boundary。

完成标准：不同 backend 在 CommitContract 上一致，外部引擎可确定性重放。

## M8 — Sampling, Parallelism and Visualization

- SimPoint / statistical sampling；
- ASI/MSI warmup；
- synchronization domain；
- conservative PDES；
- distributed experiments；
- system → component → request → cycle Top-down UI；
- operational / layered queueing view。

完成标准：并行结果与单线程确定性基准一致，性能结果可追溯到请求和队列。

## 学习推进规则

每个任务按以下顺序进行：

```text
为什么需要
→ 用自己的话预测行为
→ 最小实验
→ 观察失败或输出
→ 补齐原理
→ 写测试
→ 合入主干
```

不机械照抄大段实现，也不为了赶进度跳过失败、调试和解释。
