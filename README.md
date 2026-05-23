# silival-exercises

Systems-C exercises and reference solutions for the 25-day silicon-validation interview-prep plan. Companion to [../prep_plan/](../prep_plan/) and [EXERCISES.md](EXERCISES.md). **No DSA, no LeetCode** — every exercise is a bit/CSR/MMIO/FSM/PM/PCIe model tied to a specific day.

## Scope

- C ramp: `memcpy`/`memmove`, struct layout, pointers, linked lists.
- MMIO / `volatile` / polling / W1C / RMW primitives.
- Macro hygiene, CSR modeling, `container_of`, aligned allocator.
- Cache simulator, MESI step machine, 4-level page walk, TLB + IOMMU.
- DMA descriptor + barrier + doorbell pattern; SPSC ring with acq/rel.
- CPU bring-up ladder, pipeline + BP, license-level model.
- DVFS / P-state, PkgC gate, UPF power domain with SAVE/ISO/RESTORE asserts.
- Thermal + RAPL + TMA; PM coverage tracker; residency-delta MSR parser.
- ADR opcode model; LTSSM walker; PCIe enumeration; ASPM/L1.2 FSM; LTSSM stuck classifier; PkgC blocker arbiter.

Each exercise has a `d00_*.c` solution under [solutions/](solutions/) and is buildable via the top-level [Makefile](Makefile).

## Layout

```
EXERCISES.md     — problem statements + acceptance criteria
macros.h         — shared macros (containerof, BIT, BUILD_BUG_ON, etc.)
Makefile         — builds all solutions into bin/
solutions/       — reference implementations (d00_*.c)
exercises/       — empty templates to solve yourself
bin/             — build output (gitignored)
```

## Build

```sh
make           # build all
make clean
./bin/sol-d00_my_memcpy
```

Tested on macOS (clang/aarch64). Should build on Linux x86_64 unchanged.

## License

MIT
