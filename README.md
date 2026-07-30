# ArchLab RVSoC-Sim v2

> 面向现代处理器、SoC、虚拟化与云系统研究的，多精度、模块化、可验证、可观测的高性能全系统模拟平台。

RVSoC-Sim v2 不是为了重复实现一个缩小版 gem5，也不是为了重新造一套 QEMU。项目的目标是把成熟模拟器中最重要、最稳定的设计原则组合成一套能够长期演进的研究平台：

- 以 **Simics / gem5 / QEMU** 的全系统模拟与组件化思想为主干；
- 以 **SystemC/TLM** 的多抽象级建模方式定义组件边界；
- 以 **NEMU / Spike / RTL commit trace** 构建差分验证闭环；
- 以 **SimBricks / SST / FireSim** 的协同与并行仿真方法作为远期扩展方向；
- 将体系结构模拟、系统性能建模和真实服务器黑盒测量连接起来。

## 项目最终要做成什么

RVSoC-Sim v2 的长期目标是支持同一个全系统 SoC 在不同组件、不同时间窗口和不同研究问题下采用不同精度：

```text
CPU      = QEMU TCG / KVM fast-forward / Functional / OOO / RTL
Cache    = Functional / Approximately-timed / Cycle-accurate
NoC      = Transaction-level / Cycle-accurate / External simulator
Memory   = Fixed-latency / Queueing / DRAM timing model
PCIe     = Functional / Credit-and-queue timing model
Devices  = Native C++ / SystemC-TLM / Verilator / Remote co-simulation
```

目标能力包括：

- Functional、Timing、Transaction-level、Cycle-accurate；
- Execution-driven、Trace-driven、Synthetic traffic；
- 自研 RISC-V 功能前端；
- Spike / NEMU 差分参考；
- QEMU TCG 快速执行；
- 同 ISA 条件下的 KVM fast-forward；
- Cache、Coherence、NoC、DDR、PCIe、IOMMU、DMA、Interrupt；
- RISC-V H-extension 与全系统 Linux；
- SystemC/TLM、Verilator、外部模拟器协同；
- ROI、PC、时间窗口、事件条件触发的精度切换；
- Checkpoint、record/replay、确定性回归；
- 从系统到 cycle 的 Top-down 可视化与因果追踪。

## 核心设计原则

1. **功能语义只有一份可信定义。** Functional、Timing、OOO 和 RTL backend 通过统一的架构状态与提交记录验证。
2. **功能正确性与性能时序分离。** 先保证“算对”，再独立建模“什么时候完成”。
3. **精度属于组件，不属于整个模拟器。** CPU、NoC、DDR、PCIe 可以在同一次运行中使用不同精度。
4. **状态、切换、验证和 checkpoint 是平台能力。** 它们不是某个 CPU 模型的临时功能。
5. **全局 Tick 不等于任意组件的 cycle。** 每个组件通过 `ClockDomain(offset, period)` 映射自己的时钟。
6. **同样输入必须得到同样结果。** 相同时间事件使用稳定序号排序，禁止向模拟过去调度。
7. **所有性能结果都应可解释。** 延迟最终要能追溯到请求、队列、资源、依赖和模型假设。
8. **先用真实需求塑造抽象。** 不提前制造大量空类、DSL 或层级，只在下一项实验需要时扩展框架。

## v2 当前基线

v2 已经把 v1 中完成并验证过的基础能力整理成正式工程基线，后续不需要重新手写这些内容：

- `Tick`、`TimePoint(tick, phase, delta)` 与稳定 `TimeStamp(sequence)`；
- `ClockDomain(offset, period)`，并拒绝 `period == 0`；
- 确定性 `EventQueue`：绝对/相对调度、同时间稳定排序、取消、停止、禁止回到过去；
- MMIO 设备接口与有序 `MemoryMap`；
- region overlap、地址溢出、跨 region 访问检查；
- little-endian `SimpleRam`；
- `MagicDevice`：统计复位、ROI 开始/结束、dump、停止模拟；
- `StatsManager` 与 ROI 内指令/访存/分支计数；
- `FakeCpu` 基础工作负载；
- `SimObject` 最小生命周期；
- 轻量自动注册测试框架、CTest、GCC/Clang CI；
- 统一 `.clang-format`。

这部分代码的作用是提供一块已经验证过的地基。下一阶段开始，新的核心抽象与模型将由小实验逐个实现，而不是一次性生成完整成品。

## 目录

```text
include/archlab/
├── base/           # 稳定基础类型
├── sim/            # 时间、时钟域、事件内核、SimObject
├── platform/       # 地址空间与设备访问
├── devices/        # 基础设备
├── stats/          # 统计与 ROI
└── cpu/            # 当前只有基线 FakeCpu

src/                # 非模板实现
tests/              # 分模块平台契约测试
docs/               # 架构、路线图、测试与参考资料
.github/workflows/  # GCC/Clang CI
```

目标目录会随着真实模型逐步扩展到 `protocol/`、`state/`、`verify/`、`observe/`、`cosim/` 等层次，但当前不会预先放入无实现的空框架。

## 构建

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

运行基线演示：

```bash
./build/rvsoc-sim
```

## 下一步

M0.1 已完成。下一项正式实验是第一个组件连接契约：

```text
M0.2  Typed Port / Link
M0.3  MemRequest / MemResponse
M0.4  Pending transaction 与 backpressure
M0.5  Architectural State 与 CommitRecord
```

每一步都先解释“为什么需要”，再由一个最小实验验证，最后才进入平台主干。详细计划见 [`docs/ROADMAP.md`](docs/ROADMAP.md)。

## 文档

- [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md)：目标架构和六项核心契约；
- [`docs/BASELINE.md`](docs/BASELINE.md)：v2 已经完成的基础能力和行为定义；
- [`docs/ROADMAP.md`](docs/ROADMAP.md)：从当前基线到全系统 SoC 的阶段计划；
- [`docs/TESTING.md`](docs/TESTING.md)：轻量测试框架、目录和运行方法；
- [`docs/REFERENCES.md`](docs/REFERENCES.md)：课程、书籍、论文与主流模拟器参考。
