# Source Manifest

> Last updated: 2026-08-03

## 1. Raw source locations

### Google Drive books root

- Folder: `books`
- URL: `https://drive.google.com/drive/folders/1V5KYlHqoF8YzgsrA8ZNsljEvVoRq8Fbc`

Direct child domains observed under the root include：性能、技术记录、调试、网络、并行编程、安全、固件、计算机、编程、数学、外设、模拟器、操作系统、存储、learnArm、编译器、虚拟化、Rust、RISC-V、IBM Power、测试和 Paper。

### Paper folder

- URL: `https://drive.google.com/drive/folders/12go7cY-e8935_e9WkHuQVcf0K8cI256Z`
- First complete metadata scan used for the knowledge base: 103 PDF records.

### Books raw filename manifest

- File: `books_pdf_list.txt`
- Drive file ID: `1-zWScER3SoSm6DZf8IT7JITmkEypB5mI`
- Size observed: 38,206 bytes
- Records parsed as PDF filenames: 520

This raw manifest is the recovery source for the Books inventory. It remains in Drive; the repository stores normalized and reviewed derivatives.

## 2. Derived inventory snapshot

The 2026-08-03 L0 scan combined:

```text
520 Books PDF records
+103 Paper PDF records
=623 total records
```

Derived fields used during coarse screening:

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
```

The complete normalized inventory will be committed under `inventory/` in domain-sized text/CSV files. Until that split is complete, the raw Drive manifest plus this status document are the durable recovery path.

## 3. Coarse taxonomy

- Simulation & Modeling
- CPU / ISA / Microarchitecture
- Memory / Cache / DRAM / NVM / CXL
- NoC / Interconnect / Networking
- OS / Linux / Kernel / Virtual Memory
- Virtualization / QEMU / KVM / IOMMU
- Storage / PCIe / NVMe / SSD / USB
- RAS / Reliability / ECC
- Parallel / HPC / Concurrency / NUMA
- Compiler / Assembly / Binary / ISA Tools
- RTL / Digital Logic / FPGA
- AI / GPU / NPU / Heterogeneous
- Performance / Datacenter / Benchmarking
- Security / Trusted Computing
- Firmware / Boot / UEFI / BIOS
- Programming / C++ / Rust / Algorithms / Software Engineering
- Math / Statistics / Queueing / Optimization
- General / Needs Manual Review

The taxonomy may be corrected after content inspection. For example, Batch 2 moved `ModernSoC_textbook.pdf` from a CPU-primary classification to `SoC Integration / Interconnect / ESL`.

## 4. Canonical-work rule

One canonical work may map to multiple raw files, for example:

- Chinese and English editions;
- different scans or downloads;
- textbook PDF plus extracted TXT;
- numbered and unnumbered copies of the same paper;
- book plus solution manual, appendix or errata.

Raw records are never silently deleted. The canonical record points back to all raw filenames and records the selected primary edition.

## 5. Integrity limits

The original duplicate result is title-based. It does not by itself prove byte identity. Duplicate verification should use, where available:

1. Drive file ID and size;
2. bibliographic edition/ISBN/year;
3. table of contents and page count;
4. content hash after local download;
5. whether a file is a translation, solution manual, appendix or genuinely separate edition.

The status labels used by the knowledge base are:

- **canonical identity confirmed:** files are known to represent the same work or work family;
- **translation family:** same work in different languages, not treated as byte duplicates;
- **probable byte duplicate:** filename and metadata strongly match, but both file hashes have not yet been compared;
- **byte duplicate confirmed:** complete file hashes match;
- **related companion:** solution manual, errata or appendix attached to a canonical work but not a duplicate of it.

## 6. Canonical groups confirmed in Batch 2

### CPU-001 — Modern Processor Design

Raw records:

- `K0316` — `现代处理器设计 (舍恩, 利帕斯蒂) (Z-Library).pdf`
- `K0318` — `Modern Processor Design_ Fundamentals of Superscalar Processors - PDF Room(20220908093341).pdf`

Status: **translation family**. The English text inspected is the Shen/Lipasti work copyrighted 2005 and reissued in 2013.

### ARCH-001 — Computer Architecture: A Quantitative Approach, Sixth Edition

Raw family:

- `K0350` errata;
- `K0351` solution manual;
- `K0352` Chinese edition;
- `K0353` English edition;
- `K0354`–`K0363` companion Appendices D–M.

Status:

- English and Chinese files are reading editions of the same canonical textbook;
- solution manual, errata and online appendices are **related companions**, not duplicate textbooks;
- selected primary bibliographic source is the English sixth edition, ISBN `978-0-12-811905-1`.

### CPU-002 — Processor Microarchitecture: An Implementation Perspective

Raw records:

- `K0382` — `Processor Microarchitecture.pdf`
- `K0389` — `Processor_Microarchitecture.pdf`

Status: **canonical identity confirmed** as the 2011 González/Latorre/Magklis work. Byte identity remains pending.

### SOC-001 — Modern System-on-Chip Design on Arm

Raw records:

- `K0025` — `ModernSoC_textbook.pdf`
- `K0345` — `ModernSoC_textbook.pdf`

Drive metadata observed the same filename and size, `16,018,114` bytes, for two copies. Status: **probable byte duplicate**; exact two-file hash comparison remains pending.

The actual bibliographic identity is David J. Greaves, *Modern System-on-Chip Design on Arm*, Arm Education, 2021, ePDF ISBN `978-1-911531-37-1`.

Incremental canonical inventory: `inventory/batch-02-cpu-isa-ooo.csv`.
