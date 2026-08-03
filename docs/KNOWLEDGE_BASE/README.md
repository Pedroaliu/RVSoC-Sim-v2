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

- L0: filename screening;
- Triage: identify opaque files;
- Canonicalized: group copies/editions/translations/companions;
- L1: inspect actual TOC/abstract/introduction/key sections;
- L2: selected deep reading or complete short paper;
- L3: implementation/API/state-machine/validation extraction.

Triage or canonicalization is not automatically L1.

## Current authoritative inventories

### Raw inventory v4

- Manifest: `inventory/normalized-snapshot-v4.md`
- Drive ID: `1O1j7YWmhsrpYJt3hA1CY4nEJTuU18Pb9`

### A canonical groups v2

- Manifest: `inventory/a-canonical-groups-v2.md`
- Drive ID: `1F9kmdkoR6RZNfuvZI6s0tadIgUGLYCjF`

Older snapshots remain for audit.

## Current tree additions

```text
inventory/
    normalized-snapshot-v4.md
    a-canonical-groups-v2.md
    batch-08-virtualization.csv

reviews/
    batch-08-virtualization-qemu-kvm-iommu.md

topics/
    virtualization.md

decisions/
    virtualization-qemu-kvm-iommu-boundaries.md
```

Previous batch documents remain in the same directories.

## New-conversation recovery

Read in this order:

1. `docs/PROJECT_MEMORY.md`
2. `docs/CURRENT_STATE.md`
3. `docs/KNOWLEDGE_BASE/STATUS.md`
4. `docs/KNOWLEDGE_BASE/REVIEW_QUEUE.md`
5. `docs/KNOWLEDGE_BASE/inventory/normalized-snapshot-v4.md`
6. `docs/KNOWLEDGE_BASE/inventory/a-canonical-groups-v2.md`
7. topic-specific review/topic/decision files

Code and tests outrank the knowledge base. The knowledge base records source support, design rationale and review state.

## Maintenance rules

- preserve every raw record;
- never equate probable duplicate with byte identity;
- distinguish source statements, engineering inference and ArchLab decisions;
- record source gaps instead of filling them from chat memory;
- update status, queue, inventory and review after every batch;
- move stable contracts into formal design documents or `BASELINE.md` when implemented.
