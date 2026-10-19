# Silicon Validation Coding

Standalone C models paired with the 18-session curriculum in [`../prep_plan/`](../prep_plan/). The learning path progresses through six stages, from bit-level fundamentals to post-silicon power debugging. Session-to-exercise details are in [`EXERCISES.md`](EXERCISES.md) and [`../prep_plan/CODING_MAP.md`](../prep_plan/CODING_MAP.md).

## Layout

```text
EXERCISES.md   Session roadmap and build workflow
Makefile       Builds dNN_* and sNN_* exercise/solution targets
exercises/     Learner-owned starter files with TODOs and assertions
solutions/     Standalone reference programs with assertion checks
macros.h       Shared low-level C macros
bin/           Generated executables
```

## Build And Run

Requires a C11 compiler and make (for example, clang or GCC with GNU Make).

```sh
make all-ex                 # compile all starter files
make ex-s01_bitops          # compile and run one starter
make all-sol                # compile every reference program
make sol-s01_bitops         # compile and run one reference program
make clean
```

Starter programs compile before implementation; their assertions are expected to fail until the TODOs are completed. Exercise and solution targets accept both `dNN_<name>` and `sNN_<name>` names.

The Day 0 sanity exercises remain available alongside the new session-linked models. The source is MIT licensed; see [`LICENSE`](LICENSE).