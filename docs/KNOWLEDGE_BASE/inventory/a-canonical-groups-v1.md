# A-level Canonical Groups v1

> Generated: 2026-08-03  
> Purpose: replace the misleading raw A count with a canonical, milestone-review queue.

## Durable file

- File: `archlab_a_canonical_groups_v1.csv`
- Google Drive file ID: `1VwbkcSjDeJBZ070M-joYdt3XuEHBil6F`
- URL: `https://drive.google.com/file/d/1VwbkcSjDeJBZ070M-joYdt3XuEHBil6F/view`
- MIME type: `text/csv`
- SHA-256: `5b89e81fff4010bbff00708ea41a4206ff3878ec4b1c8d7c111160ca0158865b`

## Counts

```text
A raw records:                    156
A canonical groups:              121
Covered by prior L1 batches:       7 groups / 22 raw records
Remaining not-yet-L1 A groups:   114 groups / 134 raw records
```

## Remaining group distribution

```text
Memory / Cache / DRAM / NVM / CXL                    36
RAS / Reliability / ECC                              18
Simulation & Modeling                                11
OS / Linux / Kernel / Virtual Memory                 11
CPU / ISA / Microarchitecture                         8
Parallel / HPC / Concurrency / NUMA                   8
Virtualization / QEMU / KVM / IOMMU                   6
Storage / PCIe / NVMe / SSD / USB                     6
Performance / Datacenter / Benchmarking               3
AI / GPU / NPU / Heterogeneous                        2
Security / Trusted Computing                          2
Firmware / Boot / UEFI / BIOS                         1
NoC / Interconnect / Networking                       1
RTL / Digital Logic / FPGA                            1
```

## Fields in the CSV

```text
Canonical_ID
Canonical_Title
Raw_IDs
Raw_Filenames
Primary_Domain
ArchLab_Module
Target_Review_Stage
Coverage_Status
Canonical_Relationship
Confidence
Notes
```

## How to use it

- `Coverage_Status=covered-L1` means a previous batch already reviewed the canonical work.
- `Coverage_Status=not-yet-L1` forms the active milestone queue.
- Multiple raw IDs are preserved for translations, editions, copies and companions.
- Confidence describes canonical identity, not the quality or importance of the source.
- A priority remains provisional until L1 review confirms its actual value.

## Next group

The next batch consumes the six `Virtualization / QEMU / KVM / IOMMU` canonical groups, then writes actual L1 results back to this inventory lineage.
