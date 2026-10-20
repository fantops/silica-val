# Silicon Validation Coding

Standalone Systems-C models and assertion-checked reference solutions, organized into six stages of silicon validation and systems engineering. The exercise roadmap and session mapping are in [`EXERCISES.md`](EXERCISES.md).

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
make check-sol              # compile and run every runnable reference
make sol-s01_bitops         # compile and run one reference program
make clean
```

Starter programs compile before implementation; their assertions are expected to fail until the TODOs are completed. Curriculum exercises use `sNN_<topic>` filenames so files sort in session order; the legacy Day 0 set keeps its `d00_*` names.

The C programs are bounded teaching models, not RTL timing, metastability, PCIe compliance, or real DMA tests. S03 models an even-N divider and ideal safe clock selection; S04 models request/ack progress and delayed Gray-pointer visibility; S09 also runs a two-thread FIFO/wraparound test. S12 is a single-threaded memory-ring illustration, not a portable hardware-MMIO implementation. S17 thresholds describe a fictional platform policy. Real hardware needs its own specification, accessors, mapping/ownership rules, and instrumentation.

Keep assertions enabled. `check-sol` runs references; `all-sol` only builds them. `d00_bench` links against `d00_csr_model`; `d00_csr_init` is an excluded legacy draft. Use sanitizer-enabled builds when supported; passing tests does not prove every weak-memory interleaving or physical CDC property.

The Day 0 sanity exercises remain available alongside the new session-linked models. The source is MIT licensed; see [`LICENSE`](LICENSE).