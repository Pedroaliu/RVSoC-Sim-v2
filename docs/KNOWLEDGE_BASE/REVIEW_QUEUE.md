# Knowledge Review Queue

> Last updated: 2026-08-03

每批完成后立即落库。Triage、canonicalization 和 L1/L2/L3 分开计数。

## Completed

### L1 source batches

- Batch 1 Simulation foundation：8；
- Batch 2 CPU / ISA / OOO / SoC：4；
- Batch 3 Memory / Coherence / NoC：5；
- Batch 4 Performance / Datacenter / Methodology：4；
- Batch 5 Linux MM / VM：4；
- Batch 5B RISC-V Linux boot contracts：6；
- Batch 8 Virtualization / QEMU / KVM / IOMMU：6；
- Batch 9 PCIe / NVMe / SSD / CXL：7；
- Batch 10 RAS / ECC / fault injection / recovery：18。

L1 total: **62 canonical source families**.

### Classification batches

- Batch 6: 43 R records resolved; R=0.
- Batch 7: 156 A raw records → 121 canonical groups.

Current A coverage after Batch 10：

```text
covered-L1:       38 groups / 56 raw records
not-yet-L1:       83 groups / 100 raw records
```

Current manifest：`inventory/a-canonical-groups-v4.md`。

## Batch 10 — RAS / ECC / Chipkill / Fault Injection / Recovery

Status: **L1 complete**

Artifacts：

- `reviews/batch-10-ras-ecc-reliability.md`
- `topics/ras.md`
- `decisions/ras-fault-recovery-boundaries.md`
- `inventory/batch-10-ras-ecc.csv`
- `inventory/normalized-snapshot-v6.md`
- `inventory/a-canonical-groups-v4.md`

RAS / Reliability / ECC now has zero not-yet-L1 A groups.

## Batch 11 — Remaining Memory / Cache / DRAM / NVM / CXL

Status: **next / 35 A canonical groups**

This is too broad for one undifferentiated review. First split the 35 groups into canonical sub-batches, then review each subgroup immediately and commit it independently.

Planned order：

1. cache hierarchy, MSHR/prefetch/replacement and cache-model supplements；
2. memory-controller scheduling, DRAM organization/timing and simulator backends；
3. refresh, RowHammer/disturbance, retention and memory reliability supplements；
4. NVM/PCM/persistent-memory architecture and durability；
5. remaining CXL memory, pooling, tiering, placement and migration sources；
6. cross-check with existing Ramulator, memory-fidelity, Linux-MM, RAS and PCIe/CXL decisions。

Extraction goals：

- separate functional memory state, queue/controller state and device/media timing；
- define address mapping, bank/rank/channel/subchannel geometry and scheduling boundaries；
- preserve request identity across cache/MSHR/controller/backend transformations；
- distinguish latency, bandwidth, queueing, row-buffer locality and interference；
- integrate refresh, scrub, faults and retirement without merging RAS state into normal transaction state；
- define NVM persistence/durability boundaries separately from volatile completion；
- retain topology, home-agent, controller and device saturation for CXL；
- record calibration/validation targets and fidelity limits。

Completion artifacts will be split by subgroup under `reviews/`, `topics/`, `decisions/` and `inventory/` rather than waiting for all 35 groups.

## Remaining milestone groups after Batch 11

- Simulation & Modeling：11；
- OS / Linux / Kernel / Virtual Memory：11；
- CPU / ISA / Microarchitecture：8；
- Parallel / HPC / Concurrency / NUMA：8；
- Performance / Datacenter / Benchmarking：3；
- AI / GPU / NPU / Heterogeneous：2；
- Security / Trusted Computing：2；
- Firmware / Boot / UEFI / BIOS：1；
- NoC / Interconnect / Networking：1；
- RTL / Digital Logic / FPGA：1。

## B policy

429 B raw records remain available. They enter review only to support a milestone, citation chain, workload or unresolved design question.
