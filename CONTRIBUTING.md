# Contributing

RVSoC-Sim v2 is developed in small, testable steps. A change is complete only when its behavior is
covered by tests and its architectural boundary is clear.

## Local workflow

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Format changed C++ files with the repository-level `.clang-format`.

## Engineering rules

1. Keep functional semantics separate from timing policy.
2. Do not let components mutate another component's private state directly.
3. Never schedule an event in simulated time earlier than the current time point.
4. Preserve deterministic ordering for events with equal simulated time.
5. Distinguish global `Tick` from a component's local clock cycle.
6. New behavior requires a focused test before it becomes part of the platform contract.
7. Avoid large scaffolding-only commits. Add abstractions when a real model needs them.

## Commit style

Use short imperative subjects, for example:

```text
Add clock-domain edge mapping
Reject overlapping memory regions
Preserve same-time event ordering
```
