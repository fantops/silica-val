# 18-Session Coding Roadmap

Systems-C exercises paired with the six-stage silicon validation curriculum. Each exercise has a learner starter in `exercises/` and a standalone, assertion-checked reference in `solutions/`. Read the matching session note first, implement the TODOs, then compare behavior with the reference.

The theory and session sequence live in [`../prep_plan/`](../prep_plan/); the companion exercise map is [`../prep_plan/CODING_MAP.md`](../prep_plan/CODING_MAP.md).

## Roadmap

| Stage | Session | Focus | Exercise | Model exercised |
|---|---|---|---|---|
| 1. Silicon Fundamentals | S01 | Signal, logic, state | [s01_bitops.c](exercises/s01_bitops.c) | Safe masks, fields, rotations, and bit counts |
| 1. Silicon Fundamentals | S02 | Timing and FSMs | [s02_fsm_detect.c](exercises/s02_fsm_detect.c) | Overlapping sequence detection and illegal-state recovery |
| 1. Silicon Fundamentals | S03 | Clocks and resets | [s03_clock_reset.c](exercises/s03_clock_reset.c) | Glitchless source selection, division, ordered reset release |
| 2. Domains, Power-Up, Boot | S04 | Metastability and CDC | [s04_cdc_sync.c](exercises/s04_cdc_sync.c), [s04_async_fifo.c](exercises/s04_async_fifo.c) | Toggle/handshake crossings and Gray-pointer FIFO behavior |
| 2. Domains, Power-Up, Boot | S05 | Power-on sequence | [s05_dev_init.c](exercises/s05_dev_init.c) | Bounded status polling and register bring-up |
| 2. Domains, Power-Up, Boot | S06 | Firmware boot flow | [s06_cpu_bringup_ladder.c](exercises/s06_cpu_bringup_ladder.c) | Ordered boot prerequisites and retry budget |
| 3. Hardware-Software Interface | S07 | Registers and MMIO | [s05_dev_init.c](exercises/s05_dev_init.c), [s07_reg_poke.c](exercises/s07_reg_poke.c) | Volatile accesses, RMW preservation, bounded polling |
| 3. Hardware-Software Interface | S08 | Memory and caches | [s08_cache_sim.c](exercises/s08_cache_sim.c) | Tag/set decode, locality, set conflicts, LRU |
| 3. Hardware-Software Interface | S09 | Coherence and ordering | [s09_spsc_acqrel.c](exercises/s09_spsc_acqrel.c) | SPSC ownership and acquire-release publication |
| 4. PCIe and DMA | S10 | PCIe link-up and LTSSM | [s10_ltssm_walk.c](exercises/s10_ltssm_walk.c) | Legal LTSSM transitions and BAR size decoding |
| 4. PCIe and DMA | S11 | Enumeration and driver probe | [s11_enum_walker.c](exercises/s11_enum_walker.c) | Recursive bus traversal, bridge windows, capabilities |
| 4. PCIe and DMA | S12 | DMA, doorbells, interrupts | [s12_sq_doorbell.c](exercises/s12_sq_doorbell.c) | Descriptor ordering, bounded ring, MMIO doorbell |
| 5. SVA and Accelerators | S13 | IOMMU, PASID, ATS | [s13_tlb_iommu.c](exercises/s13_tlb_iommu.c) | TLB hits, IOVA translation, permission faults |
| 5. SVA and Accelerators | S14 | Accelerator bring-up | [s05_dev_init.c](exercises/s05_dev_init.c), [s02_fsm_detect.c](exercises/s02_fsm_detect.c) | Device handshake, status polling, safe state handling |
| 5. SVA and Accelerators | S15 | Accelerator job end-to-end | [s12_sq_doorbell.c](exercises/s12_sq_doorbell.c) | Generic descriptor submission and completion consumption |
| 6. Power and Post-Silicon Debug | S16 | Gating and DVFS | [s16_dvfs.c](exercises/s16_dvfs.c), [s16_residency_delta.c](exercises/s16_residency_delta.c) | Safe voltage/frequency order and wrap-safe counters |
| 6. Power and Post-Silicon Debug | S17 | C-states, S0ix, device PM | [s17_pkgc_blocker_arbiter.c](exercises/s17_pkgc_blocker_arbiter.c) | Package idle voting and blocker explanations |
| 6. Power and Post-Silicon Debug | S18 | Post-silicon debug capstone | [s16_residency_delta.c](exercises/s16_residency_delta.c) | Residency deltas, percentages, sanity checks |

## Build And Run

Use a C11 compiler with GNU Make, from this directory:

```sh
make all-ex
make ex-s01_bitops
make all-sol
make sol-s01_bitops
make clean
```

`all-ex` compiles every starter; its assertions are expected to fail until the TODOs are implemented. `all-sol` compiles each runnable reference program. Targets use `ex-<name>` or `sol-<name>` and accept both `dNN_<name>` and `sNN_<name>` prefixes.

## Exercise Contracts

- `s07_reg_poke`: preserve neighboring register bits and distinguish poll success from timeout.
- `s05_dev_init`: perform reset, bounded ready polling, control-bit updates, and data readback.
- `s01_bitops`: cover zero/overflow boundaries, field preservation, rotate-width guards, and 64-bit composition.
- `s02_fsm_detect`: detect overlapping and non-overlapping `101` in `10101`; recover from an illegal state.
- `s03_clock_reset`: switch only with both clock inputs low, divide on clock edges, and release reset domains in order.
- `s04_cdc_sync`: show a lost back-to-back toggle event and lossless ordered handshake transfer.
- `s04_async_fifo`: exercise full/empty rejection, wraparound, and FIFO data ordering.
- `s08_cache_sim`: show four compulsory misses for sequential accesses and conflict misses for a fixed-set stride.
- `s13_tlb_iommu`: demonstrate a TLB hit, valid IOVA mapping, absent mappings, and write-protection faults.
- `s12_sq_doorbell`: publish descriptors before updating the doorbell and consume queue entries in order.
- `s09_spsc_acqrel`: enforce empty/full behavior and ordered publication with C11 acquire-release atomics.
- `s06_cpu_bringup_ladder`: reject stage skips, enforce retry limits, and reach the final boot stage.
- `s16_dvfs`: apply voltage-before-frequency on ramp-up and frequency-before-voltage on ramp-down; enforce thermal caps.
- `s16_residency_delta`: handle unsigned counter wrap and flag residency greater than elapsed wall ticks.
- `s10_ltssm_walk`: accept legal link-state transitions, count Recovery entries, and size a BAR from readback.
- `s11_enum_walker`: discover the fake four-function topology recursively and size/program a bridge window.
- `s17_pkgc_blocker_arbiter`: distinguish full PkgC10 eligibility from PCIe and USB blockers.

Day 0 sanity exercises remain in `solutions/d00_*.c`; they are foundations rather than part of the 18-session mapping.