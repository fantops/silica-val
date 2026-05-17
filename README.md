# silival-exercises

C exercises and reference solutions for silicon validation interview prep — focused on the systems-software fundamentals that come up in CPU/SoC post-silicon and bring-up roles.

## Scope

- Memory model exercises (memcpy/memmove, struct sizes, alignment).
- MMIO / volatile / polling primitives.
- Linked-list / cycle detect / pointer fluency.
- Macro hygiene and CSR (control/status register) modeling.
- Microbenchmark scaffolds.

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
