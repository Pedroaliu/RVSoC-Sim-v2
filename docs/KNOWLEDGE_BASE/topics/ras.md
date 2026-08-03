# Topic Map — RAS, ECC, Fault Injection, and Recovery

## Layered model

```text
Physical / injected fault
    ↓ manifestation and masking
Component error state
    ↓ detection and correction
Architectural RAS event
    ↓ containment and notification
Firmware / OS recovery policy
    ↓
service, isolation, retirement, repair
```

## Fault taxonomy

- duration: transient / intermittent / permanent;
- scope: bit / burst / symbol / word / line / page / device / link / unit;
- origin: core, cache, NoC, DRAM, PCIe/CXL, storage, accelerator, firmware;
- observability: masked / corrected / detected / silent;
- severity: recoverable / contained / degraded / fatal;
- correlation: independent / spatially correlated / temporally correlated / common-mode.

## ECC submodel

```text
protected payload + geometry
    ↓ encoder
codeword
    ↓ fault pattern
received word
    ↓ checker/decoder
syndrome + location + outcome
```

Policies include parity, SEC/SECDED, adjacent-bit codes, BCH, Reed–Solomon and symbol-oriented Chipkill-style arrangements. The code policy is separate from DIMM/rank/device mapping.

## Recovery submodel

```text
retry
scrub / corrected writeback
line or page delete
page retirement
spare substitution
rank/device/core deconfiguration
checkpoint rollback / process recovery
service action
```

## Observability

- raw fault and syndrome stream;
- corrected/uncorrected counters by physical location;
- propagation path and masking point;
- recovery action and latency;
- workload/ROI and injection manifest;
- AVF/DVF/SDC/availability results with provenance.

## Cross-project ownership

- RVSoC-Sim: component fault propagation, ECC, SoC containment, PCIe/CXL/DRAM RAS timing and full-system recovery hooks;
- ArchLab-virt: VMM/KVM error delivery, guest/host containment, passthrough/IOMMU error experiments;
- shared: fault schema, severity taxonomy, traces, deterministic test vectors and recovery-result format.
