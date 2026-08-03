# Decision — RAS Fault, Detection, and Recovery Boundaries

## Status

Accepted knowledge-base design direction. Implementation remains ordered behind the current simulator-kernel and functional-platform milestones.

## Decisions

1. A fault is not an error; an error is not automatically a failure; an injected event is not automatically detected.
2. Every fault carries stable identity, origin, physical/logical location, duration, correlation group and activation rule.
3. Detection produces a separate immutable RAS event containing syndrome, affected range, confidence and severity.
4. Correction, retry, scrub, repair, retirement, deconfiguration and notification are separate actions with explicit latency and outcomes.
5. ECC is a pluggable component contract. Codeword geometry and memory-device mapping are configuration, not hidden constants.
6. Public transaction identity remains `TransactionKey`; fault/ECC/scrub private pointers never replace it.
7. CE counters and adaptive thresholds are policy state. Raw events remain available for replay and alternative-policy evaluation.
8. Fault injection campaigns are deterministic from manifest + seed and must declare workload, ROI, sample population, repetitions and convergence criteria.
9. Hardware, firmware, OS and service-processor responsibilities remain layered and observable, following the project's POWER-inspired product-level RAS target.
10. Application-aware protection and ABFT are optional experimental policies, not baseline correctness assumptions.

## Core data objects

```text
FaultSpec
FaultInstance
RasEvent
Syndrome
RecoveryAction
RepairState
RetirementRecord
InjectionManifest
```

## Required invariants

- one-shot faults activate at most once;
- persistent faults remain active until repair/deconfiguration;
- a corrected error can still update CE history and trigger policy;
- an uncorrectable event does not imply silent corruption if containment succeeds;
- recovery completion is distinct from request completion;
- checkpoint/replay reproduces active faults, pending scrub/recovery events and counters;
- no host pointer or container address appears in serialized RAS state.
