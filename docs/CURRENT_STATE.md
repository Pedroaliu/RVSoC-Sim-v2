# Current State

> Last updated: 2026-08-02

这份文档是项目的工作接力点。每个里程碑结束、分支切换或重要设计决定落地后都应更新它。

## 1. 当前工作线

- Repository: `Pedroaliu/RVSoC-Sim-v2`
- Active development branch: `lab/m0.3-transaction-completion`
- Current milestone: **M0.3 Transaction / Completion**
- Current documentation branch: `docs/project-memory`

长期目标见 `docs/PROJECT_MEMORY.md`，已完成契约见 `docs/BASELINE.md`，总体顺序见 `docs/ROADMAP.md`。

## 2. 已完成

### M0.1 SimObject lifecycle

已完成最小生命周期：

```text
Constructed → Initialized → Ready → Running
```

对象具有稳定名字，不可复制/移动；public 非虚函数检查状态，protected hook 由派生组件实现。

### M0.2 Typed Port / Link

已完成：

- `InputPort<Message>`；
- `OutputPort<Message>`；
- 显式 `Link<Message>`；
- Link 自身是 `SimObject`；
- 一源一宿和连接完整性检查；
- 未连接输出端禁止发送；
- `InputPort::receive()` 仅允许 Link 调用；
- 类型安全的同步结构投递；
- GCC、Clang 和 CTest 基线。

当前调用链是：

```text
OutputPort::send()
→ Link::transmit()
→ InputPort::receive()
→ receiver-owned handler
```

当前 Link 没有 latency、queue、capacity、credit、accepted/retry 或 EventQueue 投递。

## 3. M0.3 已确认的设计

### 3.1 事务身份

```text
TransactionKey = (source_id, request_id)
```

原因：相同地址可以有多个 outstanding request；不同 source 可以使用相同 request ID；`request_id` 只需在同一个 source 当前 outstanding 集合内唯一。

### 3.2 请求和响应是两条路径

```text
Requester.req_out → request_link → Responder.req_in
Requester.rsp_in  ← response_link ← Responder.rsp_out
```

Response 不是 `send()` 的函数返回值。Completion 是事务生命周期结果；Response 是其传输表示。Request 中不嵌入 callback，以保留序列化、checkpoint、跨进程和生命周期安全。

### 3.3 Accepted / RetryLater 语义

- `RetryLater`：接收方没有接收，不保留状态，不产生副作用；发送方保留所有权，可用同一个 key 重试。
- `Accepted`：接收方已经接收，并欠一个终态 Completion；发送方不能因为等待时间长就重复发送。

Timeout 是观测，不是丢失证明，也不自动返还请求所有权。Cancel、query、dedup、epoch/reset recovery 留给后续 RAS 设计。

### 3.4 同步重入

由于 M0.2 Link 立即调用 handler，可能发生：

```text
Requester::issue()
  └─ request_out.send(request)
      └─ Responder::on_request(request)
          └─ response_out.send(response)
              └─ Requester::on_response(response)
```

执行 `on_response()` 时，外层 `issue()` 仍暂停在 `send()` 内。这不是多线程，而是嵌套函数调用。

必须遵守：

1. send 前先登记 pending；
2. 不保存 iterator、pointer 或 reference 跨过可能触发 callback 的 send；
3. send 返回后按 key 重新 find；
4. 正常完成只由 `on_response()` 删除 pending；
5. send 抛异常时，`issue()` 按 key 回滚并重新抛出。

概念流程：

```cpp
pending_requests.emplace(key, PendingState::Sending);

try {
    request_out.send(request);
} catch (...) {
    pending_requests.erase(key);
    throw;
}

const auto current = pending_requests.find(key);
if (current != pending_requests.end()) {
    current->second.state = PendingState::Outstanding;
}
```

`on_response()` 可以合法地看到：

- `Sending`：响应在原 request send 返回之前同步到达；
- `Outstanding`：响应在以后异步到达。

未知 key 或重复 Completion 是协议错误，不能临时创建 fake pending entry。

## 4. 当前尚未实现

M0.3 分支目前还没有正式的 Transaction/Completion 类型。尚未实现：

- `SourceId`；
- `RequestId`；
- `TransactionKey`；
- Request / Response 最小类型；
- PendingEntry / PendingState；
- 同步与异步 Completion 测试；
- 乱序 Completion；
- unknown / duplicate Completion 检测。

## 5. 下一步准确动作

从最小身份类型开始，不先写完整 Requester/Responder：

1. 新建 `include/archlab/sim/transaction.h`；
2. 决定 `SourceId` 和 `RequestId` 使用简单 alias 还是强类型 wrapper；
3. 定义最小 `TransactionKey`；
4. 决定 pending 容器先使用 `std::map` 还是 `std::unordered_map`；
5. 写最小单元测试：相等、不等、作为 key、重复 key 被拒绝；
6. 测试通过后再定义 Request/Response 和 pending lifecycle。

当前学习问题是：**先比较简单 alias 与强类型 wrapper 的收益和成本，再由用户自己选择并实现第一版。**

## 6. M0.3 必须覆盖的最终测试

- 同步立即 Completion；
- EventQueue 延迟 Completion；
- 两个请求乱序完成；
- 相同 request ID、不同 source 正确区分；
- duplicate outstanding key 被拒绝；
- unknown Response 报错；
- duplicate Completion 报错；
- send 抛异常时 pending 回滚；
- send 内同步完成时无 iterator use-after-erase；
- Completion 恰好一次地结束 pending transaction。

## 7. 当前刻意不做

- queue capacity 和实际 backpressure；
- retry scheduling；
- timeout replay；
- cancel、dedup 和 recovery epoch；
- CHI、PCIe、GPU 特定字段；
- OOO、Linux 或动态 fidelity switching。

这些内容必须建立在 M0.3 的通用事务契约之上。
