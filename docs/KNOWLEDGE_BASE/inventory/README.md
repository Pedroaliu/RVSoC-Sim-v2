# Inventory Policy

The knowledge base inventory is built incrementally by review batch.

Current batch files:

- `batch-02-cpu-isa-ooo.csv`
- `batch-03-memory-coherence-noc.csv`
- `batch-04-performance-methodology.csv`

Each row maps a raw Drive file to a canonical work and records edition/duplicate relationships. Raw records are retained even when they are probable duplicates.

The full 623-record domain inventory is not yet committed as a single monolithic CSV. The recovery sources remain:

- Google Drive `books_pdf_list.txt` for 520 Books records;
- the Paper folder metadata scan for 103 Paper records;
- `../SOURCE_MANIFEST.md` and `../STATUS.md` for counts and scan state.

As R-class triage and A-class reviews proceed, records will be appended to domain-oriented inventories. Once canonicalization is sufficiently stable, these incremental files will be consolidated into:

```text
books.csv
papers.csv
duplicates.csv
canonical-works.csv
```
