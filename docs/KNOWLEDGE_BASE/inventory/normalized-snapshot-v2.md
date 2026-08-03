# Normalized Inventory Snapshot v2

> Generated: 2026-08-03  
> Scope: all 623 raw PDF records after Batch 6 R-class resolution.

## Durable file

- File: `archlab_all_records_normalized_v2.csv`
- Google Drive file ID: `1VB6a64pttPvORjXf2Tq0ENDgoNlUi4Ou`
- URL: `https://drive.google.com/file/d/1VB6a64pttPvORjXf2Tq0ENDgoNlUi4Ou/view`
- MIME type: `text/csv`
- Local generation size: 307 KiB
- Data rows: 623, plus one header row
- SHA-256: `9bff0c5af46214e949637b5df7f17eee473c5935ee6d3983b84a7d380ba121fe`

The SHA-256 identifies the exact snapshot used by the knowledge-base status documents. A later snapshot must use a new version name and checksum rather than silently replacing this identity.

## Columns

```text
ID
Source
Source_Order
Filename
Category
Priority
Shortlist
ArchLab_Module
Read_Stage
Reason
Duplicate_Group
Review_Status
Canonical_ID
Canonical_Title
Triage_Basis
Evidence
Canonical_Relationship
Disposition
```

## Raw-record counts after Batch 6

```text
S:  36
A: 157
B: 429
C:   1
R:   0
Total: 623
```

The change from the original coarse counts is entirely due to resolving the 43 R records:

- 12 R records became A;
- 31 R records became B;
- none became S or C.

## Interpretation limits

- These are **raw-record** counts, not canonical-work counts.
- Multiple raw records can represent translations, editions, duplicate downloads, appendices or companions of one work.
- `Review_Status=Filename-only coarse classification` does not mean the content was read.
- `Review_Status=R-resolved` means the ambiguity was removed; only rows whose basis says first pages, TOC or abstract were inspected have content-level triage evidence.
- L1/L2/L3 review status is tracked in batch review documents, not inferred from priority alone.

## Recovery rule

A future conversation reconstructing the knowledge base should read:

1. `../STATUS.md`;
2. `../REVIEW_QUEUE.md`;
3. this snapshot manifest;
4. the Drive CSV identified above;
5. relevant batch review and topic/decision documents.
