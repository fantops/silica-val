# 18-Session Coding Roadmap

Systems-C exercises paired with the six-stage silicon validation curriculum. Each exercise has a learner starter in `exercises/` and a standalone, assertion-checked reference in `solutions/`. Read the matching session note first, implement the TODOs, then compare behavior with the reference.

This guide is self-contained and maps each coding exercise to its session topic, starter source, and acceptance behavior.

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

## How To Work An Exercise

1. Open the linked starter and read its TODOs and assertions in `main()`.
2. Implement the TODOs without weakening or removing the assertions.
3. Run the named `make ex-...` target. A starter is expected to fail its assertions until its TODOs are complete.
4. Once it passes, run the matching `make sol-...` target and compare its behavior with the reference.

S14 reuses `s05_dev_init` and `s02_fsm_detect`; S15 reuses `s12_sq_doorbell`. The sessions build on those models rather than requiring duplicate source files.

## Exercise Tasks

### S01: `s01_bitops.c`

Implement the 32-bit bit toolkit: power-of-two checks, floor/ceiling powers of two, Kernighan and SWAR popcounts, field extraction/update, rotations, bit/byte reversal, leading/trailing-zero counts, and XOR helpers. Guard zero inputs, invalid field widths, shifts by the word size, and `ceil_pow2` overflow.

The assertions check boundary values, neighboring-bit preservation, rotation round trips, bit-count agreement, XOR cases, and 64-bit composition. Run `make ex-s01_bitops`.

### S02: `s02_fsm_detect.c`

Implement a parameterized sequence detector, a sliding-window comparison oracle, and a safe FSM step. Preserve matching suffixes when overlap is enabled; return to the reset state for injected or invalid states.

The harness checks that `101` occurs twice in `10101` with overlap and once without, and that invalid states recover. Run `make ex-s02_fsm_detect`.

### S03: `s03_clock_reset.c`

Implement a clock-mux step that changes sources only while both clocks are low, a divide-by-N counter that advances on rising edges, and ordered reset release with invalid-order detection.

The assertions check a glitch-free switch, divide-by-two behavior, and sequential domain release. Run `make ex-s03_clock_reset`.

### S04: `s04_cdc_sync.c`

Model an open-loop toggle synchronizer sampled every `dest_div` source cycles, a closed-loop ordered word transfer, and binary-to-Gray conversion. The toggle model should deliver separated events but lose two toggles that occur between destination samples; the handshake must deliver every word.

The harness checks both event patterns, ordered word delivery, and a Gray-code result. Run `make ex-s04_cdc_sync`.

### S04 Stretch: `s04_async_fifo.c`

Implement Gray pointers, full/empty detection with a wrap bit, and FIFO read/write operations that reject overflow and underflow without losing or inventing data. Complete the burst-backlog model used to explore FIFO depth.

The assertions check full/empty behavior, data order across the ring, rejected operations, and a one-bit Gray transition. Run `make ex-s04_async_fifo`.

### S05: `s05_dev_init.c`

Complete the fake-device initialization path. Implement masked register updates and a bounded ready poll that simulates hardware asserting ready on the third read. Keep the reset, enable, start, and data-readback sequence intact.

The assertions check successful readiness, preserved control bits, and data readback. Run `make ex-s05_dev_init`.

### S06: `s06_cpu_bringup_ladder.c`

Implement stage advancement for the reset-to-boot ladder. Reject skipped or invalid stages, require the previous stage to be ready, and track whether a failure remains within its retry budget.

The harness attempts an illegal skip, exercises retry exhaustion, then advances through every stage to `STG_BOOT_REACHED`. Run `make ex-s06_cpu_bringup_ladder`.

### S07: `s07_reg_poke.c`

Implement field-mask creation, register read-modify-write, and bounded polling through `volatile` accessors. Reject invalid bit ranges and modify only the requested field.

The assertions check that writing `0xAA` into bits 15:8 changes `0xDEADBEEF` to `0xDEADAAEF`, and distinguish a poll timeout from an already-set bit. Run `make ex-s07_reg_poke`.

### S08: `s08_cache_sim.c`

Implement the 8-set, 4-way, 64-byte-line cache model. Decode tag and set, record hit/miss counts, refresh recency on a hit, and evict the least-recently-used line on a miss.

The harness expects four compulsory misses and 60 hits for sequential word accesses; a 1024-byte stride should conflict in one set and miss every time. Run `make ex-s08_cache_sim`.

### S09: `s09_spsc_acqrel.c`

Implement the single-producer/single-consumer ring using the provided C11 atomic indices. Use acquire/release ordering so payload writes happen before publishing the tail and payload reads happen before releasing the head. Keep the indices on separate cache lines.

The assertions check empty/full rejection and FIFO ordering across the ring. Run `make ex-s09_spsc_acqrel`.

### S10: `s10_ltssm_walk.c`

Implement the legal LTSSM transition table and count entries into Recovery. Also calculate BAR size from the readback mask after ignoring BAR type bits.

The harness walks a legal path, rejects `Detect -> L0`, checks two Recovery entries, and verifies a 1 MiB BAR result. Run `make ex-s10_ltssm_walk`.

### S11: `s11_enum_walker.c`

Use the provided four-function fake topology to implement recursive bus enumeration. Recurse through bridges, size BARs, and assign an aligned bridge memory window while preserving discovered MSI/MSI-X capability data.

The assertions check four discovered functions, BAR size, bridge-window programming, and the endpoint with MSI-X. Run `make ex-s11_enum_walker`.

### S12: `s12_sq_doorbell.c`

Implement submission and device-consume operations for the 16-entry ring. The descriptor is fixed at 64 bytes. Publish descriptor fields before the release fence and MMIO doorbell update; consume entries in order and report empty/full conditions.

The harness checks doorbell advancement, descriptor contents and order, and empty-queue behavior. Run `make ex-s12_sq_doorbell`.

### S13: `s13_tlb_iommu.c`

Implement FIFO TLB lookup with deterministic page-walk results, then the two-level IOMMU translation. Distinguish missing first-level entries, missing second-level mappings, and writes to read-only pages.

The assertions check a repeat-access TLB hit, a valid IOVA-to-PA translation, both missing-map paths, and a permission fault. Run `make ex-s13_tlb_iommu`.

### S16: `s16_dvfs.c`

Select the lowest operating point that meets the requested frequency, subject to the 1200 MHz cap at temperatures of 90 C or higher. Model safe transition order: voltage before frequency when increasing performance, frequency before voltage when decreasing it. Calculate IPC safely when the cycle count is zero.

The harness checks normal and thermally capped selections, transition order, and IPC. Run `make ex-s16_dvfs`.

### S16: `s16_residency_delta.c`

Implement unsigned 64-bit wrap-safe counter deltas and residency percentages for the core and package counters. Flag a residency delta greater than elapsed TSC ticks and print a compact summary row.

The assertions check a wrap from near `UINT64_MAX`, a valid percentage, and an impossible over-100% residency. Run `make ex-s16_residency_delta`.

### S17: `s17_pkgc_blocker_arbiter.c`

Aggregate core, PCIe/LTR, USB, display, and memory-controller idle votes. Return the deepest eligible package state and write a useful explanation into the supplied reason buffer when a vote blocks deeper idle.

The harness checks full PkgC10 eligibility, a shallow PCIe link limiting entry to PkgC6, and active USB limiting entry to PkgC2. Run `make ex-s17_pkgc_blocker_arbiter`.

Day 0 sanity exercises remain in `solutions/d00_*.c`; they are foundations rather than part of the 18-session mapping.