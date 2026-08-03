# A-level Canonical Groups v2

> Generated: 2026-08-03  
> Change: marks all six Batch 8 Virtualization/QEMU/KVM/IOMMU groups as L1-covered.

## Durable file

- File: `archlab_a_canonical_groups_v2.csv`
- Google Drive file ID: `1F9kmdkoR6RZNfuvZI6s0tadIgUGLYCjF`
- URL: `https://drive.google.com/file/d/1F9kmdkoR6RZNfuvZI6s0tadIgUGLYCjF/view`
- SHA-256: `c04b2b41a0f281cba832a0d84e0c1cd4b0bc10097a9ea2dfd777234f230a1989`

## Counts after Batch 8

```text
A canonical groups total:        121
Covered by L1 batches:            13 groups / 28 raw records
Remaining not-yet-L1:            108 groups / 128 raw records
```

Batch 8 covered:

- VIRT-001 RISC-V IOMMU Architecture Specification;
- VIRT-002 QEMU embedded/device-model guide;
- VIRT-003 KVM实战与原理解析;
- VIRT-004 Hardware Support for Efficient Virtualization;
- VIRT-005 AMD IOMMU Specification;
- VIRT-006 QEMU/KVM源码解析与应用.

The RISC-V IOMMU source is promoted from A to S in the normalized knowledge interpretation because it is an authoritative implementation contract. The A-canonical lineage retains it as a covered historical queue entry so coverage accounting remains reproducible.

## Recovery

Use this v2 manifest instead of v1 when calculating remaining A work. v1 remains an audit snapshot of the queue before Batch 8.
