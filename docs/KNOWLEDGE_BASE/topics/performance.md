# Topic Map — Performance Evaluation and Datacenter Scope

> Last updated: 2026-08-03

This topic map links performance-evaluation sources to ArchLab framework and experiment responsibilities.

## 1. Core source hierarchy

### General methodology

- Raj K. Jain, *The Art of Computer Systems Performance Analysis*
  - question/metric/workload selection;
  - common mistakes;
  - experimental design;
  - statistics and confidence;
  - simulation verification/validation;
  - queueing and operational laws.

### Analytical modeling

- Mor Harchol-Balter, *Performance Modeling and Design of Computer Systems*
  - open/closed systems;
  - Little's Law and operational laws;
  - utilization, response time and saturation;
  - server farms, scheduling and variability;
  - capacity provisioning.

### Architecture-specific evaluation

- Lieven Eeckhout, *Computer Architecture Performance Evaluation Methods*
  - metrics for single-thread, multithread and multiprogram workloads;
  - representative workload reduction;
  - empirical/mechanistic modeling;
  - functional, trace-driven, execution-driven and full-system simulation;
  - sampled/statistical/parallel/accelerated simulation.

### Warehouse-scale boundary

- Barroso, Hölzle and Ranganathan, *The Datacenter as a Computer*, Third Edition
  - WSC as one system;
  - workload/software/hardware integration;
  - storage/network/power/cooling;
  - monitoring, tail behavior, failures and cost.

## 2. ArchLab experiment object

An ArchLab experiment should be representable as:

```text
Experiment
├── question
├── scope
│   ├── service/system/component boundary
│   └── effects intentionally excluded
├── workload
│   ├── binary/image/trace/generator
│   ├── input set
│   └── representativeness evidence
├── configuration
│   ├── resolved simulator config
│   └── per-component fidelity
├── execution
│   ├── warmup
│   ├── ROI
│   ├── seed
│   ├── repetitions
│   └── termination rule
├── metrics
│   ├── raw counters
│   ├── derived metrics
│   ├── normalization
│   └── aggregation/confidence
├── validation
│   ├── deterministic oracle
│   ├── analytical bound
│   ├── external simulator
│   └── hardware measurement
└── conclusion
    ├── supported range
    └── limitations
```

## 3. Metric layers

### Functional correctness

- architectural state mismatch;
- protocol violation;
- unknown/duplicate completion;
- deadlock/livelock;
- checkpoint replay mismatch.

### Component timing

- latency distribution;
- queue occupancy;
- service rate/utilization;
- stall cycles by cause;
- retries/backpressure;
- bandwidth and saturation point.

### Core/memory/system performance

- instructions/cycles/IPC;
- MPKI and miss latency;
- MLP/outstanding requests;
- throughput and fairness;
- normalized turnaround;
- energy or power where modeled.

### Service/datacenter

- request throughput;
- p50/p95/p99/p999 latency;
- SLO violations;
- utilization/capacity headroom;
- availability/failure/recovery;
- cost and energy per unit of work.

A metric must name its scope and aggregation. An average alone must not hide tail or fairness behavior when those affect the question.

## 4. Evaluation-mode map

| Mode | Appropriate questions | Important exclusions |
|---|---|---|
| Functional | ISA/platform/device semantics, Linux boot, software-visible behavior | microarchitectural timing and contention |
| Fixed latency | transaction lifecycle, deterministic delay plumbing | saturation, state-dependent scheduling, latency distributions |
| Queue/analytical | capacity, utilization, first-order saturation and bounds | detailed protocol/device state unless modeled |
| Trace-driven | isolated policy comparison under fixed request stream | timing feedback, wrong-path and software adaptation |
| Execution-driven timing | pipeline/cache/NoC/memory feedback | depends on component fidelity |
| Full-system | OS, drivers, interrupts, synchronization and realistic software | may be slow; detailedness still component-specific |
| External co-simulation | detailed target component with fast surrounding system | synchronization/adapter validity must be established |
| Hardware measurement | calibration and end-to-end ground truth | limited visibility and control; platform-specific noise |

## 5. Validation ladder

```text
Unit invariant
→ deterministic micro-test
→ analytical law/bound
→ cross-model comparison
→ external simulator comparison
→ hardware calibration
→ application/system validation
```

Examples:

- queue: arrivals − departures = occupancy change;
- pending transaction: accepted request receives exactly one terminal completion;
- Little's Law: average occupancy approximately equals throughput × average residence time in steady state;
- cache: known address pattern produces expected hit/miss sequence;
- NoC: zero-load path latency matches pipeline/link model;
- DRAM: command legality and timing checked independently of application IPC;
- full system: workload behavior and bottleneck trends compared with hardware or a trusted reference.

## 6. Top-down visualization requirements

The visualization must preserve both hierarchy and provenance:

```text
Service / experiment objective
  ↓
Workload / phase / ROI
  ↓
VM / process / thread / function
  ↓
Top-down stall class
  ↓
Core / cache / coherence / NoC / DRAM / PCIe
  ↓
Queue / request / transaction
  ↓
Event / cycle
```

Clicking or zooming must not change the definition of the underlying metric. The UI should reveal:

- raw numerator and denominator;
- time/ROI range;
- selected workload entities;
- component fidelity;
- whether data is measured, simulated, estimated or derived;
- uncertainty and missing effects.

## 7. Near-term implementation implications

Before advanced visualizations, ArchLab should first provide:

1. resolved configuration output;
2. run metadata and reproducibility manifest;
3. stable counter and histogram APIs;
4. ROI and phase identifiers;
5. raw-event trace with controlled filtering;
6. derived-metric layer separate from collection;
7. validation tests and analytical sanity checks;
8. machine-readable result export.

## 8. Later detailed review queue

- Raj Jain: experimental design, simulation validation and ratio/averaging pitfalls at L2;
- Harchol-Balter: Little's Law, M/M/1, M/M/k, server farms and variability at L2;
- Eeckhout: workload reduction, sampled simulation and interval models at L2;
- Datacenter third edition: tail tolerance, failures, accelerators and monitoring at L2 when distributed workloads begin.
