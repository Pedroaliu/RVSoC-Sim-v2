# Testing

RVSoC-Sim v2 当前使用一个项目内置的轻量测试注册框架，并由 CTest 统一运行。

这样做的目的不是重新开发一套复杂测试库，而是在 M0 阶段保持：

- 无第三方依赖；
- 测试注册机制透明，便于学习 C++ 编译、链接和静态注册；
- 每个模块拥有独立测试文件；
- CLion、命令行和 CI 使用同一套 CMake/CTest 入口。

## 目录结构

```text
tests/
├── test_framework.h
├── test_main.cpp
├── sim/
│   ├── test_time.cpp
│   ├── test_clock_domain.cpp
│   ├── test_event_queue.cpp
│   └── test_sim_object.cpp
├── devices/
│   └── test_simple_ram.cpp
├── platform/
│   └── test_memory_map.cpp
└── integration/
    └── test_roi_fake_cpu.cpp
```

`test_main.cpp` 不包含其他 `.cpp` 文件。每个测试源文件分别编译，最后由链接器组成同一个 `archlab-tests` 可执行程序。

每个 `ARCHLAB_TEST()` 宏会创建一个静态 `Registrar`，程序进入 `main()` 前将测试函数登记到统一注册表。`main()` 只负责遍历注册表并运行全部测试。

## 添加测试

创建一个放在 `tests/` 下面的 `.cpp` 文件：

```cpp
#include "archlab/sim/sim_object.h"
#include "test_framework.h"

using archlab::sim::LifecycleState;
using archlab::sim::SimObject;

ARCHLAB_TEST("SimObject starts constructed") {
    const SimObject object("object0");

    CHECK(object.state() == LifecycleState::Constructed);
}
```

CMake 使用：

```cmake
file(GLOB_RECURSE ARCHLAB_TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
)
```

因此新测试文件会被自动加入 `archlab-tests`。`CONFIGURE_DEPENDS` 会要求 CMake 在测试文件集合变化时重新配置。

## 当前断言

```cpp
CHECK(expression);
CHECK_THROWS_AS(expression, exception_type);
```

失败时测试框架抛出异常，记录当前测试失败，然后继续运行其余测试。

## 构建和运行

```bash
cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBUILD_TESTING=ON

cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

也可以直接运行测试程序，查看每个测试的名字：

```bash
./build/archlab-tests
```

## 约束

- 单元测试文件只测试一个清晰模块；
- 跨组件场景放入 `tests/integration/`；
- 新功能先写正常路径，再补非法输入和状态不变量；
- 测试不得依赖执行顺序；
- 不把 `.cpp` 文件通过 `#include` 拼接起来；
- 纯测试重构不得改变产品代码行为。

当后续需要 fixture、参数化测试或更丰富的报告时，再评估迁移到 GoogleTest/Catch2；当前框架只服务于 M0 的透明性和低依赖目标。
