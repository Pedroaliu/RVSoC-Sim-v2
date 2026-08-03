# Decision — PCIe, NVMe, SSD, and CXL Model Boundaries

## Status

Accepted knowledge-base design direction. Implementation details remain gated on versioned normative specifications and tests.

## Decisions

1. PCIe is split into software-visible function/configuration state, transaction semantics, data-link reliability and physical/link timing.
2. PCIe link Ack/Nak never completes an ArchLab device request.
3. Credit exhaustion is transport backpressure; it does not return protocol ownership or become `RetryLater`.
4. `TransactionKey` remains the public cross-subsystem identity. PCIe Requester ID/Tag are adapter-level identities; internal allocator/replay/credit objects are private.
5. MaxPayload, MaxReadReq, tags, credits and completion fragmentation are resolved run configuration and result metadata.
6. The NVMe controller frontend and SSD media backend are replaceable components with an explicit command/durability boundary.
7. Guest SQ/CQ entries and controller registers are architectural/platform state. FTL tables, NAND queues, GC tasks and media-wear state are private model state but checkpointable.
8. Hotplug is asynchronous and cross-layer; object creation/deletion alone is insufficient.
9. CXL memory has separate topology, transport, controller and media components; fixed latency is only one low-fidelity backend.
10. Restricted/NDA-marked sources may be registered for provenance but detailed contents are not committed to the public repository.

## Initial fidelity ladder

```text
PCIeFunctional
PCIeTransactionTimed
PCIeCreditAndTagTimed
PCIeLinkDetailed

NVMeFunctionalController
NVMeQueuedController
SSDPolicyBackend
SSDNandTimedBackend

CXLFixedLatency
CXLQueuedBandwidth
CXLComponentTimed
```

Each experiment must name the selected fidelity per component.
