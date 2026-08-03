# Knowledge Base Status

> Last updated: 2026-08-03

这是 ArchLab 资料工作的跨对话接力点。raw asset、canonical group、triage、L1 review 和工程决定分别记录。

## 1. 全量资产

623 条 PDF raw records：Books 520，Paper 103。

当前优先级：

```text
S:  37
A: 155
B: 429
C:   2
R:   0
Total: 623
```

当前 raw inventory：

- `inventory/normalized-snapshot-v5.md`
- Drive ID `1xfHcOVBdAb9yMdr9xnk4vZWDASoIpB2Y`
- SHA-256 `a67cd9899e5d7285f49072d7204d8943a0cf17dd091eb0c92808be175f442b97`

v5 没有改变优先级数量；它把 Batch 9 的内容审阅状态、canonical identity 和 restricted-source disposition 写回 8 条 raw records。

## 2. L1 内容审阅

现有 **44 个 canonical works/source families** 达到 L1：

- Batch 1 Simulation foundation：8；
- Batch 2 CPU / ISA / OOO / SoC：4；
- Batch 3 Memory / Coherence / NoC：5；
- Batch 4 Performance / Datacenter / Methodology：4；
- Batch 5 Linux MM / Virtual Memory：4；
- Batch 5B RISC-V Linux boot contracts：6；
- Batch 8 Virtualization / QEMU / KVM / IOMMU：6；
- Batch 9 PCIe / NVMe / SSD / CXL：7。

Batch 6、7 分别是 R triage 与 A canonicalization，不计入 L1 数量。

## 3. Canonical coverage

### Batch 6 — R complete

```text
43 raw R → 32 canonical groups → R=0
```

### Batch 7 — A canonicalization complete

```text
156 A raw records → 121 A canonical groups
```

### Batch 9 后的 A coverage

- manifest：`inventory/a-canonical-groups-v3.md`
- Drive ID：`15_kV0hTWIVaLCKUWq7ip5gN_D9-GEipd`
- SHA-256：`46f3ddf1cd881ec85c0f4be14c980eae9aeb675b162f69d535effbf2960fb321`

```text
A canonical groups total:       121
L1 covered:                      20 groups / 36 raw records
Remaining not-yet-L1:           101 groups / 120 raw records
```

Remaining domains：

```text
Memory / Cache / DRAM / NVM / CXL                    35
RAS / Reliability / ECC                              18
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

The original Storage / PCIe / NVMe / SSD / USB A group is now fully covered at L1.

## 4. Batch 9 — PCIe / NVMe / SSD / CXL

Status: **L1 complete**

Reviewed:

1. *PCI Express Technology: Comprehensive Guide to Generations 1.x, 2.x, 3.0*；
2. *PCI Express 体系结构导读*；
3. *A Beginner's Guide to SSD Firmware*；
4. *深入浅出SSD（第二版）*；
5. restricted vendor NVMe hotplug source（L1-restricted）；
6. *PCM-Based Durable Write Cache for Fast Disk I/O*；
7. *Performance Characterization of CXL Memory and Its Use Cases*。

Records:

- `reviews/batch-09-pcie-nvme-ssd-cxl.md`
- `topics/pcie-storage-cxl.md`
- `decisions/pcie-nvme-ssd-cxl-boundaries.md`
- `inventory/batch-09-pcie-storage-cxl.csv`

Batch CSV Drive ID: `1CgEL4GpOrTBdYH2S0ZTWKNqEBsorDyoT`; SHA-256 `76cd81417dc4b1b9ac6de54c58da37b59342bfee202e0b0f6ad56945971e5bf9`.

## 5. Accepted PCIe/storage/CXL direction

### PCIe layers

```text
function/configuration
transaction request/message/completion
credit transport + data-link Ack/Nak/replay
physical link + LTSSM
```

- link Ack/Nak is not transaction completion；
- posted writes do not receive transaction completions；
- non-posted requests consume tag identity and later receive completion(s)；
- credit exhaustion is transport backpressure, not `RetryLater`；
- MaxPayload/MaxReadReq/tag/credit/completion fragmentation are timing-model inputs；
- `TransactionKey` remains public identity; Requester ID/Tag are PCIe-adapter identity; replay/credit/LTSSM objects remain private。

### NVMe and SSD

- guest-visible controller registers, SQ/CQ, doorbells, namespaces and interrupts are separated from FTL/NAND/GC/ECC/wear/power-loss private state；
- host command completion follows command ordering/durability semantics, not merely internal media scheduling；
- queue entries are guest-visible memory; controller work entries and media queues are private but checkpointable；
- functional NVMe controller, queued controller, SSD policy backend and NAND-timed backend are separate fidelity levels。

### Hotplug

Presence, slot power, link state, configuration visibility, outstanding I/O, error containment and OS/firmware ownership are separate asynchronous states. A device cannot be hot-removed by simply deleting an object.

### CXL

- CXL memory retains socket/home-agent topology, transport, device-controller queueing, media and bandwidth saturation；
- CXL Type-3 memory is not modeled as only “DRAM + constant latency”；
- placement/interleaving/migration and workload sensitivity are part of validation；
- the reviewed paper is a performance source for real CXL 1.1 devices, not a normative protocol source。

## 6. Restricted-source policy

One Batch 9 file is visibly marked AMD Confidential / NDA Required. The public repository records only provenance, review status and high-level category. Detailed contents, quotes and vendor-specific procedures are intentionally excluded. Implementation must use public normative PCIe/ACPI/Linux sources.

## 7. Explicit normative gaps

Before implementation, pin versioned sources for：

1. PCI Express Base Specification and relevant ECNs/capabilities；
2. NVM Express Base and selected command-set specifications；
3. CXL specification for the chosen version/device type；
4. public PCIe hotplug, AER/DPC, ACPI and Linux behavior。

## 8. Next batch

Batch 10: **RAS / ECC / Chipkill / fault injection / recovery**. There are 18 remaining A canonical groups in this domain. Review will start with coding/ECC foundations, DRAM field-failure evidence, Chipkill/SDDC-style organization, fault injection and recovery/validation.

Accurate state:

> All 623 raw assets are normalized; R=0; 44 source families have L1 review; 20 of 121 A groups are covered and 101 remain. PCIe/NVMe/SSD/CXL boundaries are recorded, and the entire storage-domain A group is covered at L1.
