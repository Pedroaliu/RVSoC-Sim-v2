# References

本项目不照抄单个模拟器。下面资料分别承担不同角色。

## 课程与书籍

- Grigory Rechistov, Evgenii Iuliugin et al., *Программное моделирование вычислительных систем*；
- Evgenii Iuliugin, *Fundamentals of Full-Platform Simulation* lecture series；
- David A. Patterson, John L. Hennessy, *Computer Architecture: A Quantitative Approach*；
- Erol Gelenbe (ed.), *Computer System Performance Modeling in Perspective*；
- James E. Smith, Ravi Nair, *Virtual Machines: Versatile Platforms for Systems and Processes*；
- 处理器、Cache、NoC、虚拟化、Linux 与性能分析相关课程资料。

## 工业与研究平台

### Simics

参考：完整虚拟平台、对象化设备、checkpoint/replay、脚本控制、DML 与执行模式切换。

### gem5

参考：SimObject、事件驱动内核、CPU model switching、Standard Library、Ruby/SLICC。

### QEMU

参考：QOM/qdev 生命周期、MemoryRegion、TCG、设备模型、迁移状态与 qtest。

### NEMU / Spike / XS-GEM5

参考：功能 golden model、commit-level difftest、采样与体系结构验证闭环。

### SystemC / TLM

参考：module/port/channel、loosely-timed、approximately-timed、temporal decoupling。

### SST / SimBricks

参考：组件图、同步域、并行离散事件和进程级协同仿真。

### FireSim / Verilator

参考：host/target 解耦、RTL 加速与软件模拟器到 RTL 的验证边界。

## 官方入口

- https://www.gem5.org/
- https://github.com/qemu/qemu
- https://github.com/OpenXiangShan/NEMU
- https://github.com/riscv-software-src/riscv-isa-sim
- https://systemc.org/
- https://sst-simulator.org/
- https://simbricks.github.io/
- https://fires.im/
- https://www.veripool.org/verilator/

引用这些系统不意味着接口兼容或代码派生。RVSoC-Sim v2 只吸收经过验证的架构思想，并维护自己的稳定契约。
