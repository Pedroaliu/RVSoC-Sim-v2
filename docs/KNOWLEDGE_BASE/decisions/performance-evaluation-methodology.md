# Decision — Performance Evaluation and Result Provenance

> Status: accepted project direction  
> Date: 2026-08-03  
> Evidence level: L1 source review; implementation details remain future work

## Context

ArchLab will support multiple execution modes and component fidelities. The same workload may run with a functional core, timing core, trace front end, fixed-latency memory, detailed DRAM, message-level NoC or flit-level NoC. Without explicit experiment metadata, the resulting numbers can be compared outside their valid scope.

The project also aims to provide Top-down visualization from a cloud workload down to individual cycles. This creates a risk that attractive dashboards hide metric definitions, normalization choices, missing effects or causal uncertainty.

## Decision

Every ArchLab performance result must be associated with a resolved experiment manifest and explicit provenance.

Minimum experiment metadata:

```text
research_question
system_boundary
workload_and_input
resolved_configuration
per_component_fidelity
warmup_and_roi
seed_and_repetitions
metric_definitions
normalization_and_aggregation
validation_reference
known_exclusions
```

## Collection and analysis are separate layers

```text
Simulation model
    ↓ emits
Raw counters / histograms / events
    ↓ transformed by
Derived metric definitions
    ↓ interpreted by
Experiment analysis and report
```

The simulator core should not silently bake policy conclusions into counters. Raw data must remain available for checking derived metrics.

## Accuracy statement

Results must describe accuracy relative to the research question:

- functional correctness;
- timing plumbing;
- queueing/capacity approximation;
- detailed component timing;
- full-system software interaction;
- warehouse-scale service behavior.

No model is labeled simply “accurate” without naming which effects it represents and which it omits.

## Statistical rules

- Deterministic correctness tests do not require repeated stochastic confidence intervals.
- Noisy hardware measurements and simulations with randomized inputs require repetitions, seeds and uncertainty reporting.
- Averages, ratios and normalized results must state the baseline and aggregation rule.
- Tail latency, fairness and availability must be reported when relevant; the mean alone is insufficient.
- Warmup, ROI and termination criteria are part of the result, not incidental run settings.

## Validation rules

Model verification asks whether the implementation matches its specification.

Model validation asks whether the specification is adequate for the intended real or reference system.

Validation should proceed through the strongest practical ladder:

```text
invariants
→ deterministic micro-tests
→ analytical laws/bounds
→ cross-model comparison
→ external simulator
→ hardware measurement
→ application/system trends
```

## Analytical-model role

Little's Law, operational bounds and simple queueing models are mandatory sanity checks where their assumptions apply. They do not replace protocol, device-state or timing-feedback simulation.

## Top-down visualization rule

Every displayed metric must be traceable to:

- raw numerator/denominator or source events;
- time/ROI interval;
- selected workload entities;
- component fidelity;
- derivation/normalization rule;
- uncertainty and unsupported effects.

Zooming changes the level of detail, not the metric's hidden definition.

## Consequences

### Positive

- results remain reproducible across conversations and branches;
- different fidelity modes can be compared honestly;
- dashboards can support debugging rather than merely decoration;
- analytical and hardware validation can be integrated systematically;
- performance claims retain a clear applicability range.

### Cost

- each run needs additional metadata;
- stats and visualization require schema/version discipline;
- some experiments need repeated runs and calibration;
- results lacking provenance may be rejected even if they look plausible.

## Sources

- Raj K. Jain, *The Art of Computer Systems Performance Analysis*;
- Mor Harchol-Balter, *Performance Modeling and Design of Computer Systems*;
- Lieven Eeckhout, *Computer Architecture Performance Evaluation Methods*;
- Barroso, Hölzle and Ranganathan, *The Datacenter as a Computer*, Third Edition.

Detailed source notes: `../reviews/batch-04-performance-datacenter-methodology.md`.
