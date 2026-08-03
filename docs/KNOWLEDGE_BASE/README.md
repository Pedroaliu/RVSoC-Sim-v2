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

### Raw inventory v5

- Manifest: `inventory/normalized-snapshot-v5.md`
- Drive ID: `1xfHcOVBdAb9yMdr9xnk4vZWDASoIpB2Y`
- SHA-256: `a67cd9899e5d7285f49072d7204d8943a0cf17dd091eb0c92808be175f442b97`

### A canonical groups v3

- Manifest: `inventory/a-canonical-groups-v3.md`
- Drive ID: `15_kV0hTWIVaLCKUWq7ip5gN_D9-GEipd`
- SHA-256: `46f3ddf1cd881ec85c0f4be14c980eae9aeb675b162f69d535effbf2960fb321`

Older snapshots remain for audit.

## Current tree additions

```text
inventory/
    normalized-snapshot-v5.md
    a-canonical-groups-v3.md
    batch-09-pcie-storage-cxl.csv

reviews/
    batch-09-pcie-nvme-ssd-cxl.md

topics/
    pcie-storage-cxl.md

decisions/
    pcie-nvme-ssd-cxl-boundaries.md
```

Previous batch documents remain in the same directories.

## New-conversation recovery

Read in this order:

1. `docs/PROJECT_MEMORY.md`
2. `docs/CURRENT_STATE.md`
3. `docs/KNOWLEDGE_BASE/STATUS.md`
4. `docs/KNOWLEDGE_BASE/REVIEW_QUEUE.md`
5. `docs/KNOWLEDGE_BASE/inventory/normalized-snapshot-v5.md`
6. `docs/KNOWLEDGE_BASE/inventory/a-canonical-groups-v3.md`
7. topic-specific review/topic/decision files

Code and tests outrank the knowledge base. The knowledge base records source support, design rationale and review state.

## Restricted-source rule

A source visibly marked confidential, NDA-required, export-restricted or otherwise non-public may be registered for provenance. The public repository must not contain detailed extracts, proprietary procedures or long quotations from that source. Public normative sources are required before implementation.

## Maintenance rules

- preserve every raw record；
- never equate probable duplicate with byte identity；
- distinguish source statements, engineering inference and ArchLab decisions；
- record source gaps instead of filling them from chat memory；
- update status, queue, inventory and review after every batch；
- move stable contracts into formal design documents or `BASELINE.md` when implemented。
