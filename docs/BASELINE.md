# v2 Baseline Contract

这份文档定义当前代码已经完成的行为。后续实验可以依赖这些行为，不需要重新实现 v1 内容。

## 1. 时间

- `Tick` 是无符号 64 位全局时间；
- `TimePoint` 按 `tick, phase, delta` 字典序比较；
- `TimeStamp` 在 `TimePoint` 后增加 `sequence`；
- `next_phase()` 增加 phase 并清零 delta；
- `next_delta()` 增加 delta；
- phase 或 delta 溢出时抛出异常。

## 2. ClockDomain

构造参数是：

```cpp
ClockDomain(offset, period)
```

边沿公式：

```text
edge(cycle) = offset + cycle × period
```

已保证：

- `period == 0` 立即拒绝；
- edge 计算溢出立即拒绝；
- 可以判断某个 Tick 是否为时钟边沿；
- 可以计算不早于指定 Tick 的下一个边沿。

## 3. EventQueue

已保证：

- 禁止向当前 `TimePoint` 之前调度；
- 相同 `TimePoint` 按创建顺序稳定执行；
- callback 在当前时间创建的同时间事件排在已有同时间事件之后；
- 支持绝对 Tick、完整 TimePoint 和相对 Tick 调度；
- `schedule_after(0)` 被解释为当前 phase 的下一个 delta；
- 支持 event handle 取消；
- 支持 stop/continue；
- `run_until(Tick)` 处理该 Tick 内所有 phase 和 delta；
- trace 输出包含 tick、phase、delta、sequence、id 和 name。

## 4. MemoryMap

已保证：

- region 按地址排序；
- 不允许零大小 region；
- 不允许 region 重叠；
- 检测地址加法溢出；
- 不允许一次访问跨越两个 region；
- 未映射访问抛出异常；
- 设备收到的是相对 region base 的 offset。

## 5. SimpleRam

- little-endian；
- 支持 1/2/4/8 字节访问；
- 越界访问抛出异常；
- 当前采用连续 host vector，仅用于基线；后续会增加 sparse/page-backed RAM。

## 6. Magic / ROI / Stats

Magic command：

```text
0 Nop
1 ResetStats
2 DumpStats
3 StopSimulation
4 BeginRoi
5 EndRoi
```

统计只在 ROI 内累计。当前统计时间单位明确叫 `roi_ticks`，不叫 cycle，因为全局 Tick 不一定等于 CPU cycle。

## 7. FakeCpu

`FakeCpu` 只用于验证平台流动：

- ALU 计数；
- branch 计数；
- 64 位 load/store；
- 通过 MemoryMap 访问设备；
- 只在 ROI 内进入 StatsManager。

它不是未来 CPU 架构的基类，不应在上面继续堆 ISA 或 pipeline 功能。

## 8. SimObject 最小生命周期

每个正式模拟组件可以继承 `SimObject`，获得稳定名字和最小生命周期状态机：

```text
Constructed
    → initialize()
Initialized
    → reset()
Ready
    → startup()
Running
```

已保证：

- 名字不能为空；
- 对象不可复制、不可移动，以保持地址稳定；
- public 非虚函数负责检查状态并推进生命周期；
- protected `on_initialize()`、`on_reset()`、`on_startup()` 由派生组件定制；
- 钩子成功返回后才改变状态；
- 非法转换抛出 `std::logic_error`，且不执行设备钩子。

当前刻意不支持运行中的 reset、drain、shutdown 和 checkpoint；这些能力会在有明确实验需求时扩展。

## 9. Typed Port / Link

组件可以声明类型化端口，并通过显式 `Link<Message>` 建立一对一连接：

```text
OutputPort<Message>
        ↓
Link<Message>
        ↓
InputPort<Message>
        ↓
receiver-owned handler
```

已保证：

- `InputPort` 和 `OutputPort` 都有稳定 owner 与本地名字；
- 端口名字不能为空，输入端处理函数不能为空；
- `Link` 是有稳定名字的 `SimObject`；
- 一个 `Link` 只能连接一个输出端和一个输入端；
- 一个端口只能属于一个 `Link`；
- 连接失败不会把仍空闲的另一端留下为半连接状态；
- 未连接的输出端禁止发送；
- 消息类型由模板在编译期约束；
- 只有 `Link` 可以调用输入端处理函数，发送方不能绕过连接直接调用接收端。

M0.2 的 `Link` 只表达结构连接并立即投递消息。它刻意不包含 latency、queue、capacity、accepted/retry、backpressure 或异步 completion；这些属于下一阶段的事务契约。

## 10. 测试基线

测试使用项目内置的轻量注册框架和 CTest：

- `tests/test_main.cpp` 只负责运行注册表；
- 各模块测试放在独立 `.cpp` 文件中；
- `ARCHLAB_TEST()` 在静态初始化期间自动注册测试；
- CMake 使用 `GLOB_RECURSE ... CONFIGURE_DEPENDS` 自动发现新增测试源文件；
- 无第三方测试库依赖；
- `ctest --test-dir build --output-on-failure` 是统一入口。

详细说明见 `docs/TESTING.md`。

## 11. 基线停止的位置

当前刻意没有实现：

- `MemRequest` / `MemResponse`；
- accepted / retry / backpressure；
- pending transaction 与异步 completion；
- ArchitecturalState；
- CommitRecord；
- checkpoint；
- functional RISC-V CPU。

这些将从下一阶段开始由独立实验逐项加入，避免把未经理解的空框架提前固化。
