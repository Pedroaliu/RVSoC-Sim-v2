# Normalized Inventory Snapshot v3

> Generated: 2026-08-03  
> Scope: all 623 raw PDF records after Batch 7 A-level canonicalization corrections.

## Durable file

- File: `archlab_all_records_normalized_v3.csv`
- Google Drive file ID: `1Rcm-ivqexORPxfz07ThJe_-afTGES361`
- URL: `https://drive.google.com/file/d/1Rcm-ivqexORPxfz07ThJe_-afTGES361/view`
- MIME type: `text/csv`
- Data rows: 623, plus one header row
- SHA-256: `61388baed6db3474fefcaddb362d77647297ef404e864f0757b9159b08acd150`

The checksum identifies the exact snapshot used by Batch 7. Future corrections require a new versioned snapshot and checksum; this file must not be silently replaced.

## Raw-record counts

```text
S:  36
A: 156
B: 429
C:   2
R:   0
Total: 623
```

Changes relative to v2:

- `PMU_DATA.pdf` was content-identified as an electrical-power excitation-system paper and moved from A to C;
- opaque A filenames were assigned content-backed titles/domains where inspected;
- canonical IDs and relationships were added for the A review set;
- R remains zero.

## Interpretation limits

- Counts are raw records, not canonical works.
- Priority does not imply L1 review.
- Canonical relationships marked probable still require edition/page-count/hash verification.
- The actionable not-yet-L1 A review set is recorded in `a-canonical-groups-v1.md`.

## Recovery rule

A future conversation should read:

1. `../STATUS.md`;
2. `../REVIEW_QUEUE.md`;
3. this snapshot manifest;
4. the Drive CSV above;
5. `a-canonical-groups-v1.md`;
6. relevant review/topic/decision documents.
