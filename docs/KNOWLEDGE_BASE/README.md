# ArchLab Knowledge Base

This directory stores versioned source inventory, actual content review and engineering mappings for ArchLab RVSoC-Sim.

## Storage

### Raw PDFs — Google Drive

- Books: `https://drive.google.com/drive/folders/1V5KYlHqoF8YzgsrA8ZNsljEvVoRq8Fbc`
- Paper: `https://drive.google.com/drive/folders/12go7cY-e8935_e9WkHuQVcf0K8cI256Z`
- Raw book list ID: `1-zWScER3SoSm6DZf8IT7JITmkEypB5mI`

### Durable knowledge — GitHub

GitHub stores raw-to-canonical relationships, priority, actual reviews, topic maps, decisions, progress and queues. Chat history is not the source of truth.

## State levels

- L0: filename screening；
- Triage: identify opaque files；
- Canonicalized: group copies/editions/translations/companions；
- L1: inspect actual TOC/abstract/introduction/key sections；
- L1-restricted: content was inspected but public extraction is intentionally limited by source markings；
- L2: selected deep reading or complete short paper；
- L3: implementation/API/state-machine/validation extraction。

Triage or canonicalization is not automatically L1.

## Current authoritative inventories

### Raw inventory v6

- Manifest: `inventory/normalized-snapshot-v6.md`
- Drive ID: `1Pn3mmhEGkWySAmqOMpzha7D-hpDiUYJr`
- SHA-256: `d2a6767a8361985bde98176bdf4cf2feee91ea5f35957d873ed5ca59167d532c`

### A canonical groups v4

- Manifest: `inventory/a-canonical-groups-v4.md`
- Drive ID: `1vHK1N2Dw6jBUqA5NxvYl9xC5VopPMsDe`
- SHA-256: `cc971d4e1dd338e45bb658513e4af9bfcf219f914ca6f499c769c4ba955d750b`

Older snapshots remain for audit.

## Current tree additions

```text
inventory/
    normalized-snapshot-v6.md
    a-canonical-groups-v4.md
    batch-10-ras-ecc.csv

reviews/
    batch-10-ras-ecc-reliability.md

topics/
    ras.md

decisions/
    ras-fault-recovery-boundaries.md
```

Previous batch documents remain in the same directories.

## New-conversation recovery

Read in this order:

1. `docs/PROJECT_MEMORY.md`
2. `docs/CURRENT_STATE.md`
3. `docs/KNOWLEDGE_BASE/STATUS.md`
4. `docs/KNOWLEDGE_BASE/REVIEW_QUEUE.md`
5. `docs/KNOWLEDGE_BASE/inventory/normalized-snapshot-v6.md`
6. `docs/KNOWLEDGE_BASE/inventory/a-canonical-groups-v4.md`
7. topic-specific review/topic/decision files

Code and tests outrank the knowledge base. The knowledge base records source support, design rationale and review state.

## Restricted-source rule

A source visibly marked confidential, NDA-required, export-restricted or otherwise non-public may be registered for provenance. The public repository must not contain detailed extracts, proprietary procedures or long quotations from that source. Public normative sources are required before implementation.

## RAS-specific evidence rule

- fault, error, architectural corruption, report and recovery must remain distinct；
- a fault-injection campaign must preserve its resolved manifest and seed；
- ECC geometry, syndrome, correction capability and timing are explicit configuration；
- CE/UE counters and thresholds are policy state, while raw events remain replayable evidence；
- application masking or ABFT cannot be assumed unless explicitly enabled and validated。

## Maintenance rules

- preserve every raw record；
- never equate probable duplicate with byte identity；
- distinguish source statements, engineering inference and ArchLab decisions；
- record source gaps instead of filling them from chat memory；
- update status, queue, inventory and review after every batch；
- move stable contracts into formal design documents or `BASELINE.md` when implemented。
