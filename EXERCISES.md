# Coding — Single Exercise Document

> One folder, one document. All 42 days of coding live here. Systems-C only. Read the prompt, write the code in `exercises/dNN_<name>.c`, build, run, explain *why*, then check it against the reference.
>
> **Where the reference lives:** Day 0 has runnable files in [solutions/](solutions/). For the coding days (see the map below) the reference solution is inline in this document (under each day's heading) and is extracted into a runnable `solutions/dNN_*.c` only when you choose to — this tree fills in incrementally as you expand the knowledge base on the job.

This file is the coding companion to [prep_plan/README.md](../prep_plan/README.md). The plan is **42 days, no DSA, no LeetCode** — every exercise is a bit/CSR/FSM/CDC/memory/verification/PM/PCIe systems-C model tied to the day's reading. It is one continuous bottom-up build (Day 0–41); the [prep_plan/README.md](../prep_plan/README.md) has the phase map.

## How to use

```
coding/
├── EXERCISES.md      ← this file. Follow it top to bottom.
├── macros.h          ← shared bit/field/align macros (Day 0). Used everywhere.
├── Makefile          ← unified build for everything in exercises/ and solutions/
├── exercises/        ← YOU WRITE CODE HERE. One file per problem.
│   └── dNN_<name>.c
└── solutions/        ← reference. Peek only after attempting.
    └── dNN_<name>.c
```

Build / run a single exercise (replace `<target>` with e.g. `d00_my_memcpy`):

```sh
make ex-<target>     # build + run YOUR attempt from exercises/
make sol-<target>    # build + run the reference solution
make all-ex          # build every exercise stub (will fail until filled in)
make all-sol         # build every reference solution
make clean
```

Naming convention: every file is prefixed `dNN_` where `NN` is the day number. Day 0 = `d00_*.c`, Day 1 = `d01_*.c`, etc.

Build flags used everywhere:

```sh
cc -Wall -Wextra -Wpedantic -std=c11 -g -O0 -I. exercises/<file>.c -o /tmp/<file> && /tmp/<file>
```

## Map: what to code each study day

| Day | Topic | Exercise |
|---|---|---|
| 0 | Embedded-C basics ramp | `d00_*` (see [day00](../prep_plan/day00_c_basics.md)) |
| 1 | C address fluency | `d01_reg_poke` |
| 2 | Pointers / alignment | `d02_aligned_alloc` |
| 3 | Bits / MMIO / volatile / W1C | `d03_dev_init` |
| 4 | Bit manipulation mastery | `d04_bitops` |
| 5 | Boolean + combinational logic | `d05_comb_logic` |
| 6 | Arithmetic circuits | `d06_arith` |
| 7 | Sequential logic + flip-flops | `d07_seq_timing` |
| 8 | Timing & STA | `d08_sta_paths` |
| 9 | FSM design + sequence detectors | `d09_fsm_detect` |
| 10 | Verilog & SystemVerilog | `d10_rtl_semantics` |
| 11 | Clocking & reset architecture | `d11_clock_reset` |
| 12 | Metastability + synchronizers | `d12_metastability` |
| 13 | CDC techniques | `d13_cdc_sync` |
| 14 | Async FIFO + CDC verification | `d14_async_fifo` |
| 15 | Memory systems + ECC | `d15_mem_ecc` |
| 16 | Cache lines / locality / AMAT | `d16_cache_sim` |
| 17 | Coherence / MESI / false sharing | `d17_mesi_sim` |
| 18 | Virtual memory / page walk | `d18_page_walk` |
| 19 | TLB / IOMMU | `d19_tlb_iommu` |
| 20 | DMA / doorbells / barriers | `d20_sq_doorbell` |
| 21 | Pipeline + OoO microarch | `d21_pipeline_bp` |
| 22 | Memory consistency + atomics | `d22_spsc_acqrel` |
| 23 | CPU bring-up + IP functional val | `d23_cpu_bringup_ladder` |
| 24 | Verification planning + coverage | `d24_vplan_coverage` |
| 25 | Testbench + assertions + debug | `d25_scoreboard_crv` |
| 26 | DVFS / P-states / PVT | `d26_dvfs` |
| 27 | C-states / PkgC / S0ix | `d27_pkgc6_gate` |
| 28 | Clock + power gating + UPF | `d28_upf_domain_model` |
| 29 | Thermal / perf counters | `d29_tj_rapl_tma` |
| 30 | Pre-Si PM validation | `d30_pm_coverage_tracker` |
| 31 | Post-Si PM validation + ADR/C6 | `d31_residency_delta` (+ `d31_adr` opcode model) |
| 32 | AMBA AXI & CHI | `d32_axi_model` |
| 33 | PCIe fundamentals + LTSSM | `d33_ltssm_walk` |
| 34 | PCIe enumeration / config / MSI | `d34_enum_walker` |
| 35 | PCIe power management (ASPM/L1.2) | `d35_aspm_l12_fsm` |
| 36 | PCIe bring-up + functional val | `d36_ltssm_stuck_classifier` |
| 37 | PCIe × PM intersection | `d37_pkgc_blocker_arbiter` |
| 38 | NVMe storage-controller SoC val | — *(reuse `d20_sq_doorbell` for the queue/doorbell datapath)* |
| 39 | Logic puzzles + estimation | `d39_puzzle_sims` *(optional)* |
| 40 | Intel-role narrative | — *(narrative day; no new code)* |
| 41 | Final mock interview day | — *(re-run any 2-3 exercises cold as warm-up)* |

Each coding day below has one **Systems C exercise** with full problem statement, signatures, driver, and interview prompts. Architect drills and bug anchors live in the matching [`prep_plan/dayNN_*.md`](../prep_plan/) reading file (kept in one place to avoid duplication). Exercise sections below are in day order (Day 1 → Day 39); Days 0/38/40/41 have no dedicated exercise (see notes).

---

# Day 1 — C address fluency

## D1 — Register-poke harness (`d01_reg_poke`)

**Problem statement.** Build a simulated MMIO harness from scratch. Back a register with a single `volatile uint32_t` (stand-in for a real BAR offset). Implement bit/field macros, typed accessors, an RMW field-write, and a bounded poll loop. Drive it end-to-end: program a field, demonstrate the timeout path, demonstrate the success path.

**You must implement:**
```c
#define BIT(n)
#define MASK(msb, lsb)
#define FIELD_GET(r, msb, lsb)
#define FIELD_PREP(v, msb, lsb)

static inline uint32_t reg_read32(volatile uint32_t *r);
static inline void     reg_write32(volatile uint32_t *r, uint32_t v);
static inline void     reg_field_write(volatile uint32_t *r, int msb, int lsb, uint32_t v);
static int             wait_for_bit_set(volatile uint32_t *r, uint32_t mask, int timeout_us);
```

**Driver / tests:**
- `fake_reg = 0xDEADBEEF`, `reg_field_write(&fake_reg, 15, 8, 0xAA)`, assert `0xDEADAAEF`.
- Timeout path: `fake_reg = 0`, `wait_for_bit_set(&fake_reg, BIT(0), 5)`, assert `rc == -1`.
- Success path: `fake_reg = BIT(0)`, call again, assert `rc == 0`.

**Interview prompts:** 3 things `volatile` prevents. Why `volatile` ≠ memory barrier. Where does the timeout come from for `CSTS.RDY`? Why is RMW on a W1C status register a bug?

---

# Day 2 — Pointers, memory layout, alignment

## D2 — Aligned allocator + container_of (`d02_aligned_alloc`)

**Problem statement.** Build a manual aligned allocator that satisfies arbitrary power-of-2 alignment on top of plain `malloc`, plus a Linux-style `container_of` macro. Demonstrate both on an intrusive list. Also print `sizeof` for two structs that differ only in field order to show padding is layout-driven.

**Implement:**
```c
#define ALIGN_UP(x, a)
#define IS_ALIGNED(x, a)
#define IS_POW2(n)
#define container_of(ptr, type, member)

void *aligned_malloc(size_t size, size_t align);
void  aligned_free(void *p);
```

**Driver:**
- `aligned_malloc(4096, 64)` → assert `addr % 64 == 0`, then `aligned_free`.
- Build `struct work { int id; struct list_head node; };`, take `&w.node`, recover `&w` via `container_of`, print `back->id`.
- Compare `sizeof(struct {char a; int b; char c;})` vs reordered version; explain padding.

**Interview prompts:** Why does the aligned allocator stash the original pointer one slot before the aligned address? Why is `container_of` foundational to Linux callbacks (what does `offsetof` rely on)? What does `__attribute__((packed))` cost on ARM that it doesn't on x86?

---

# Day 3 — Bits, MMIO, volatile, polling

## D3 — Device init harness with poll + RMW (`d03_dev_init`)

**Problem statement.** Build a fake-device init harness using a small `uint8_t mmio[256]` backing array. Implement typed accessors (`reg_read`, `reg_write`, `reg_rmw`), a bounded `poll_ready` that simulates HW asserting ready after N iterations, and a top-level main that resets the device, polls for ready, enables and starts it, then writes/reads `DATA`.

**Implement:**
```c
static inline uint32_t reg_read(uint32_t off);
static inline void     reg_write(uint32_t off, uint32_t v);
static inline void     reg_rmw(uint32_t off, uint32_t mask, uint32_t v);
static bool            poll_ready(uint32_t off, uint32_t mask, uint32_t exp, uint32_t tries);
```

**Register map:** `CTRL=0x00, STATUS=0x04, DATA=0x08`. Bits: `CTRL_EN=BIT(0), CTRL_START=BIT(1), CTRL_RESET=BIT(31), STATUS_RDY=BIT(0)`.

**Driver:**
- Reset (write `CTRL_RESET` then 0).
- `poll_ready(STATUS, STATUS_RDY, STATUS_RDY, 5)` — must return true; fail = "device never ready".
- RMW set `CTRL_EN`, then `CTRL_START` (preserve other bits).
- Write `0xDEADBEEF` to `DATA` and assert readback.

**Interview prompts:** 3 ways the compiler miscompiles missing-`volatile` poll loops at `-O2`. Why is RMW on a W1C status register catastrophic? Why does `volatile` not imply a memory barrier on ARM?

---

# Day 4 — Bit Manipulation Mastery

## D4 — Bit-ops library (`d04_bitops`)

**Problem statement.** Build the reflex bit toolkit and validate every function against a brute-force oracle, with explicit tests for the UB traps.

**Implement:**
```c
int      is_pow2(uint32_t x);
uint32_t floor_pow2(uint32_t x);        /* largest 2^k <= x, x>0 */
uint32_t ceil_pow2(uint32_t x);         /* smallest 2^k >= x */
int      popcount_kern(uint32_t x);     /* x &= x-1 per set bit */
int      popcount_swar(uint32_t x);     /* branch-free */
int      ffs32(uint32_t x), clz32(uint32_t x), ctz32(uint32_t x);
uint32_t get_field(uint32_t r,int lo,int w), put_field(uint32_t r,int lo,int w,uint32_t v);
uint32_t rol32(uint32_t x,int n), ror32(uint32_t x,int n);   /* width-guarded */
uint32_t bswap32(uint32_t x), reverse32(uint32_t x);
uint32_t xor_unique(const uint32_t*a,int n), xor_missing(const uint32_t*a,int n);
uint64_t combine64(uint32_t hi, uint32_t lo);   /* widen-before-shift */
```

**Driver / tests:**
- `floor_pow2(100)==64`, `ceil_pow2(100)==128`, handle `x==0/1`.
- `popcount_kern==popcount_swar==__builtin_popcount` over many values.
- `combine64(0xDEADBEEF,0x01020304)==0xDEADBEEF01020304` (prove the un-widened `hi<<32` loses the high half).
- `reverse32` round-trips twice == identity; field RMW preserves neighbour bits.

**Interview prompts:** Floor vs ceil pow2 — the shared smear move? Why widen before `<<32`? Why is `x&(x-1)` popcount faster on sparse inputs? Find-first-set = which hardware block?

---

# Day 5 — Boolean + Combinational Logic

## D5 — Combinational-block library (`d05_comb_logic`)

**Problem statement.** Model the standard combinational blocks *from their truth tables* in C and prove each against a brute-force oracle. No clocks, no state — every function is pure.

**Implement:**
```c
int      priority_encode(uint32_t in);             /* index of HIGHEST set bit, -1 if 0 */
uint32_t mux2(uint32_t a, uint32_t b, int sel);
uint32_t mux4(uint32_t d[4], int sel);
uint8_t  add4(uint8_t a, uint8_t b, int cin, int *cout);   /* 4-bit ripple, exposes carry */
typedef enum { OP_ADD, OP_SUB, OP_AND, OP_OR, OP_XOR, OP_SLT } aluop_t;
uint8_t  alu4(uint8_t a, uint8_t b, aluop_t op, int *carry, int *ovf); /* SUB = a + ~b + 1 */
int      parity(uint32_t x);                       /* XOR reduction */
int      popcount_tt(uint32_t x);
```

**Driver / tests:**
- `priority_encode(0b01010) == 3`; `priority_encode(0) == -1`.
- `add4(0xF,0x1,0,&c)` → sum `0x0`, `cout==1`. Sweep all 256×2 inputs vs a reference adder.
- `alu4` SUB: show **signed overflow == (carry-in ≠ carry-out of MSB)** on `0x78+0x50`.
- `mux4` and `parity`/`popcount_tt` checked against brute force over all inputs.

**Interview prompts:** Why is a mux universal (Shannon expansion)? Priority vs plain encoder — when does the answer change? Signed overflow as carry-in ≠ carry-out of the MSB. Where does an incomplete `case` infer a latch?

---

# Day 6 — Arithmetic Circuits

## D6 — Arithmetic library (`d06_arith`)

**Problem statement.** Model datapath arithmetic in C and validate against native ops. Show the adder speed/area idea, the fast-multiplier structure, fixed-point reformatting, and the floating-point compare trap.

**Implement:**
```c
uint32_t add_ripple(uint32_t a, uint32_t b, int *cout, int *ovf);  /* signed-overflow = cin≠cout of MSB */
uint32_t add_cla(uint32_t a, uint32_t b, int *cout);              /* G/P expansion; == ripple result */
int64_t  mul_shiftadd(int32_t a, int32_t b);
int64_t  mul_booth(int32_t a, int32_t b);                          /* radix-4 Booth; == native */
int32_t  qmul(int32_t a, int32_t b, int n);                        /* Qm.n * Qm.n -> shift>>n + round + saturate */
int      fp_eq(float a, float b, float eps);                      /* |a-b|<eps; classify NaN/Inf */
```

**Driver / tests:** sweep random signed inputs — `add_cla == add_ripple == native`; `mul_booth == mul_shiftadd == native`; show signed overflow flag on `0x7FFFFFFF+1`; `qmul` of two `Q8.8` values round-trips with saturation; `fp_eq(0.1+0.2, 0.3, 1e-6)==1` while `==` fails; `NaN != NaN`.

**Interview prompts:** Ripple vs CLA — where's the delay? Why Booth+Wallace is the fast multiplier? Why never `==` on floats? Fixed-point reformat after multiply?

---

# Day 7 — Sequential Logic + Timing

## D7 — Sequential primitives + timing calculator (`d07_seq_timing`)

**Problem statement.** Model the clocked building blocks and the static-timing math. Assert the invariants that make each one correct (max-length LFSR, single-bit Gray transition, hold independent of clock period).

**Implement:**
```c
uint32_t lfsr_step(uint32_t state, uint32_t taps, int width);  /* Galois/Fibonacci */
uint32_t bin_to_gray(uint32_t b);
uint32_t gray_to_bin(uint32_t g);
uint32_t gray_inc(uint32_t g);                 /* +1 in Gray domain */
typedef struct { double tcq, tcomb_max, tcomb_min, tsu, th, tskew; } timing_t;
double fmax_hz(const timing_t *t);
double setup_slack(const timing_t *t, double tclk);
double hold_slack(const timing_t *t);          /* note: tclk does NOT appear */
```

**Driver / tests:**
- LFSR: from a non-zero seed, assert it visits exactly `2^width - 1` states and never `0`; show all-zeros is a fixed point.
- Gray: assert `popcount(g_n ^ g_{n+1}) == 1` for a full lap; round-trip `gray_to_bin(bin_to_gray(x))==x`.
- Timing: with `tcq=80,tcomb_max=600,tsu=50,tskew=40` (ps) print `fmax`; with a short `tcomb_min` show a **negative hold slack** that slowing `tclk` cannot fix.

**Interview prompts:** Why doesn't `tclk` appear in the hold equation? Two fixes for a hold violation. Why does Gray guarantee one-bit change? What makes an LFSR lock up?

---

# Day 8 — Timing & STA

## D8 — Multi-path STA slack calculator (`d08_sta_paths`)

**Problem statement.** Given a list of timing paths and an exceptions list, compute per-path setup/hold slack, report WNS/TNS, classify each violation, and skip declared false/async paths — a tiny static timing engine.

**Implement:**
```c
typedef struct { const char *name, *launch_clk, *capture_clk;
  double period, tcq, tcomb_min, tcomb_max, tsu, th, skew, uncertainty; } path_t;
double setup_slack(const path_t *p);     /* required - arrival; period present */
double hold_slack(const path_t *p);      /* (tcq+tcomb_min) - (th+skew+unc); period ABSENT */
int    is_async(const path_t *p, const char **async_groups, int n);  /* clock-group exception */
void   report(const path_t *paths, int n, const char **async_groups, int ng); /* WNS/TNS + fix class */
```

**Driver / tests:** a mixed set including a setup-limited path (fix="slow/pipeline"), a hold-limited path (fix="add delay"; show slowing the clock doesn't help), and a **cross-clock path that generates false violations until added to the async group**. Print WNS, TNS, and per-path pass/fail + fix.

**Interview prompts:** Why no period in hold? Which corner for hold vs setup? What if you false-path a real path?

---

# Day 9 — FSM Design + Sequence Detectors

## D9 — Sequence detector + safe FSM (`d09_fsm_detect`)

**Problem statement.** Implement a serial sequence detector two ways and prove them equivalent on random bit streams, then demonstrate a **safe FSM** recovering from an illegal state.

**Implement:**
```c
/* state-machine detector: parameterized target pattern, overlapping flag */
int seq_detect(const int *bits, int n, uint32_t pattern, int plen,
               int overlapping, int *out);    /* out[i]=1 when match ends at i */
/* sliding-window matcher: "pattern appears in the last plen bits" */
int win_match(const int *bits, int n, uint32_t pattern, int plen, int *out);
/* safe FSM: inject an illegal state code, show default->reset recovery */
int safe_fsm_step(int state, int in, int inject_illegal);
```

**Driver / tests:**
- `101` overlapping on `10101` fires **twice**; non-overlapping fires once.
- Random 10k-bit streams: assert `seq_detect` (overlapping) matches a reference sliding compare.
- Inject an out-of-range state into `safe_fsm_step`; assert next state is reset, not a hang.

**Interview prompts:** Overlapping vs non-overlapping — which back-edge differs? Moore vs Mealy output timing. Why does one-hot give a free `$onehot` assertion? Where does the longest-suffix rule come from?

---

# Day 10 — Verilog & SystemVerilog

## D10 — RTL semantics model (`d10_rtl_semantics`)

**Problem statement.** Model Verilog's 4-state logic and update semantics in C so the classic bugs become visible.

**Implement:**
```c
typedef enum { L0, L1, LX, LZ } logic4_t;                 /* 0,1,X,Z */
logic4_t and4(logic4_t a, logic4_t b), or4(logic4_t,logic4_t), not4(logic4_t), xor4(logic4_t,logic4_t);
logic4_t resolve(logic4_t a, logic4_t b);                 /* two drivers: 1+0 -> X (contention) */
void shift_nonblocking(logic4_t *q1, logic4_t *q2, logic4_t d);  /* correct 2-FF shift */
void shift_blocking(logic4_t *q1, logic4_t *q2, logic4_t d);     /* collapses to 1 flop (bug) */
int  infer_latch(const int *assigned, int ncases);       /* incomplete case -> 1 (latch) */
```

**Driver / tests:** truth tables show **X-propagation** (`X & 1 = X`) and **Z-contention -> X**; `shift_nonblocking` shifts `d` through two flops while `shift_blocking` lands `d` in both (collapse); `infer_latch` flags a case that doesn't assign every path.

**Interview prompts:** Why does blocking collapse the shift register? What produces X and how does it propagate? `always_comb` vs `always @(*)`?

---

# Day 11 — Clocking & Reset Architecture

## D11 — Clock-mux + reset sequencer (`d11_clock_reset`)

**Problem statement.** Model the clocking/reset control that CDC sits on: a glitchless clock mux, a divider, and an ordered reset release.

**Implement:**
```c
/* glitchless mux: old clock must reach a clean low before the new one starts */
int  clk_mux_step(int *state, int sel, int clkA, int clkB, int *out, int *runt);  /* runt=1 on violation */
int  div_n(int *cnt, int clk_in, int n);                 /* ÷N */
/* release N domains in required order through per-domain 2-FF reset synchronizers */
int  reset_seq(int *dom_rst, const int *order, int n, int step, int *bad_order);
```

**Driver / tests:** drive two async clocks through `clk_mux_step` across many phases — assert `runt` never fires (vs a naive combinational mux that does); `div_n` halves/thirds correctly; `reset_seq` releases in order and **flags an out-of-order release**.

**Interview prompts:** Where does the runt pulse come from in a naive clock mux? Why async-assert / sync-de-assert? What breaks if reset releases out of order?

---

# Day 12 — Metastability + Synchronizers

## D12 — MTBF + synchronizer model (`d12_metastability`)

**Problem statement.** Model the metastability math and the 2-FF synchronizer, then show with an injection harness why one flop is unsafe and two are safe for a **level**.

**Implement:**
```c
double mtbf(double t_r, double tau, double T0, double f_clk, double f_data);
/* 2-FF synchronizer: returns the level delayed by two dest cycles */
int sync2_step(int *ff1, int *ff2, int async_in);
/* injection: on a near-edge transition, randomly stall stage-1 by a cycle */
int meta_inject(int *stage, int async_in, int near_edge, unsigned *seed);
```

**Driver / tests:**
- Show `mtbf` grows exponentially with `t_r` and shrinks with `f_clk`/`f_data` (print a small sweep).
- Stream a slowly-changing level through `sync2_step`; assert the output equals the input delayed by 2, never a glitch.
- Run the injection harness many seeds: a 1-FF "sync" can emit a corrupted sample; the 2-FF structure never does.

**Interview prompts:** Which MTBF term is the design lever? Why no logic between the two flops? Why no fanout from FF1? What can a 2-FF synchronizer *not* carry?

---

# Day 13 — CDC Techniques: Pulse, Handshake, Gray, Reset

## D13 — Pulse synchronizer + handshake (`d13_cdc_sync`)

**Problem statement.** Model the crossings a 2-FF can't handle: a toggle-based **pulse** synchronizer (and demonstrate a *lost* back-to-back event), and a closed-loop **req/ack handshake** that never loses a word.

**Implement:**
```c
/* open-loop: toggle in src, 2-FF + edge-detect in dest. Returns dest pulses. */
int toggle_sync(const int *src_pulses, int n, int dest_div, int *dest_pulses);
/* closed-loop 4-phase handshake moving a word; returns words delivered in order */
int handshake_xfer(const uint32_t *words, int n, uint32_t *out);
uint32_t bin_to_gray(uint32_t b);   /* reused from D7, for pointer crossings */
```

**Driver / tests:**
- Space events out → `toggle_sync` delivers all; make events back-to-back (faster than `dest_div`) → **show a dropped event** (maps to the Intel back-to-back-wake bug).
- `handshake_xfer` delivers every word, in order, regardless of spacing.

**Interview prompts:** Why does the open-loop toggle lose back-to-back events? Why is a handshake safe for a bus without syncing the data bits? Why Gray for pointers? Reset synchronizer: why async-assert / sync-de-assert?

---

# Day 14 — Async FIFO + CDC Verification

## D14 — Async FIFO + depth calculator (`d14_async_fifo`)

**Problem statement.** Model a dual-clock FIFO with **Gray-coded** pointers and the extra-MSB full/empty logic, then reproduce the Broadcom **depth = 32** result by finding the peak backlog of a dual-rate stream.

**Implement:**
```c
typedef struct { uint32_t mem[256]; uint32_t wptr, rptr; int depth; } afifo_t; /* pointers carry +1 MSB */
int  fifo_write(afifo_t *f, uint32_t v);   /* returns -1 on full (no data loss) */
int  fifo_read (afifo_t *f, uint32_t *v);  /* returns -1 on empty (no garbage)  */
int  fifo_full (const afifo_t *f);         /* gray compare, MSB inverted */
int  fifo_empty(const afifo_t *f);
/* drive randomized write/read bursts at given avg rates; return peak backlog */
int  peak_backlog(int wr_per100, int rd_per10, int clocks, unsigned seed);
```

**Driver / tests:**
- Fill/drain/wrap: assert FIFO order + data integrity; never overflow or underflow.
- `peak_backlog(80, 8, 100000, seed)` → **≈ 32** (the equal-average-rate burst-backlog case).
- Error harness: force write-at-full / read-at-empty → flags fire, no data lost/duplicated.

**Interview prompts:** Why Gray pointers? Why the extra MSB? Why is depth a *burst* problem when averages match? Why does functional sim miss CDC bugs?

---

# Day 15 — Memory Systems + ECC

## D15 — Address decode + Hamming SECDED (`d15_mem_ecc`)

**Problem statement.** Model the structural memory view: decompose an address into `[bank|row|col|byte]`, and build a single-error-correcting / double-error-detecting code that actually corrects.

**Implement:**
```c
typedef struct { int bank_bits, row_bits, col_bits, byte_bits; } geom_t;
void addr_split(uint32_t addr, const geom_t *g, uint32_t *bank, uint32_t *row,
                uint32_t *col, uint32_t *byte);
uint32_t addr_join(uint32_t bank, uint32_t row, uint32_t col, uint32_t byte, const geom_t *g);
uint16_t ecc_encode(uint8_t data);                 /* 8 data -> 13 bits: 4 check + overall parity */
int      ecc_decode(uint16_t code, uint8_t *out);  /* 0=clean, 1=corrected, 2=double-bit (uncorrectable) */
void     march_c(uint8_t *mem, int n);             /* fault-detecting write/read passes */
```

**Driver / tests:**
- `addr_join(addr_split(x))==x` round-trips for a given geometry.
- ECC: corrupt **one** bit → `ecc_decode` returns 1 and recovers the byte; corrupt **two** → returns 2 (flagged, not miscorrected). Sweep all single-bit positions.
- Inject a stuck-at / decoder fault into `mem`; assert `march_c` detects it.

**Interview prompts:** Address vs data in one sentence. Why power-of-2 check-bit positions? How does SECDED tell 1-bit from 2-bit? What fault models does a march test catch?

---

# Day 16 — Cache lines, locality, AMAT

## D16 — Address decode + cache simulator (`d16_cache_sim`)

**Problem statement.** Software model of a set-associative cache. Decode each access into `(tag, set, offset)`, look it up across the ways, update LRU bits on hits, evict on misses. Run two access patterns to demonstrate locality vs stride collapse.

**Geometry:** `LINE = 64`, `SETS = 8`, `WAYS = 4`.

**Implement:**
```c
typedef struct { bool valid; uint32_t tag; uint8_t lru; } line_t;
static line_t cache[SETS][WAYS];
bool access(uint32_t addr);   /* return true on hit, false on miss; update LRU */
```

**Driver:**
- Sequential: `for (i=0;i<64;i++) access(i*4);` → high hit rate.
- Stride-1024: `for (i=0;i<64;i++) access(i*1024);` → near-100% miss.
- Print `hits, misses, hit%`.

**Interview prompts:** Decode `tag|set|offset` for the geometry. Why does stride > line size collapse bandwidth? LRU vs PLRU vs Random — area/perf tradeoff. Compute AMAT for `(L1: 4c, miss 5%) → (L2: 12c, miss 30%) → DRAM 200c`.

---

# Day 17 — Coherence, MESI, false sharing

## D17 — MESI step-machine + false-sharing layout check (`d17_mesi_sim`)

**Problem statement.** Model two cores' L1 lines and one main-memory cell. Implement `rd(who)` and `wr(who, v)` that mutate `(cline_t c0, c1)` according to MESI rules — including HITM (other core in M forwards and both go to S) and write-invalidate. After each step, print `C0`, `C1`, `mem`.

Also: `struct bad { int a; int b; }` vs padded `struct good { int a; char pad[60]; int b; }`. Print whether `&a` and `&b` land on the same 64-byte line.

**Implement:**
```c
typedef enum { I = 0, S, E, M } mesi_t;
typedef struct { mesi_t st; int v; } cline_t;
void rd(int who);
void wr(int who, int v);
```

**Driver:** sequence `rd(0)→E. rd(1)→both S. wr(0,42)→C0 M, C1 I. rd(1)→HITM, both S. wr(1,99)→C1 M, C0 I.` Plus the layout check.

**Interview prompts:** MESI vs MOESI vs MESIF in one sentence each. Why does false sharing cost ~50–80 cycles per write? HITM counter and `perf c2c` — what do they observe? Why does padding fail if the struct itself isn't 64-B aligned?

---

# Day 18 — Virtual memory, page faults

## D18 — 4-level page walk (`d18_page_walk`)

**Problem statement.** x86-64-style 4-level page walk in C: four 512-entry tables (`pml4`, `pdpt`, `pd`, `pt`). Each PTE carries `P` and `RW` flags plus a frame-aligned PFN. The walk takes a 64-bit VA, walks the four levels by 9-bit indices, returns either a physical address or 0 with a reason string on fault.

**Implement:**
```c
#define PG_SHIFT 12
#define IDX(va, lvl) (((va) >> (PG_SHIFT + 9 * (lvl))) & 0x1FFu)
typedef uint64_t pte_t;
#define P  (1ull << 0)
#define RW (1ull << 1)
#define PFN(p) ((p) & ~0xFFFull)

uint64_t walk(uint64_t va, const char **why);
```

**Driver:** Map `VA 0x0000 → PA 0x1000` and `VA 0x1000 → PA 0x2000`. Walk both → success. Walk `0x2000` → fault `"PT not present"`.

**Interview prompts:** Why 4 levels and not flat? Why is page offset never translated? PFN vs physical address? Page fault vs TLB miss vs cache miss in 30 sec. *(This is also the anchor exercise for the Day 40 QNN page-fault bug story.)*

---

# Day 19 — TLB, huge pages, IOMMU/SMMU

## D19 — TLB lookup with FIFO + IOMMU translate (`d19_tlb_iommu`)

**Problem statement.** Two collaborating models.

(a) **4-entry FIFO TLB** caching `vpn → pfn`. On miss, call a fake `page_walk(vpn)` (any deterministic hash), install at `next_evict`, advance index. Track `hits`/`misses`.

(b) **Two-level IOMMU per BDF.** `iommu_translate(ctx, iova, write, *pa)` walks L1 (1024 entries) → L2 (1024 entries). Faults: missing L1, missing L2, write-to-RO. Print `IOMMU FAULT BDF %04X iova %08X: <reason>`.

**Implement:**
```c
typedef struct { uint32_t vpn, pfn; bool valid; } tlb_t;
uint32_t translate(uint32_t va);

typedef struct { uint32_t pfn; bool present, writable; } io_pte_t;
typedef struct { io_pte_t pt[1024]; bool present; } io_pd_t;
typedef struct { uint16_t bdf; io_pd_t *pd[1024]; } iommu_ctx_t;
bool iommu_translate(iommu_ctx_t *c, uint32_t iova, bool wr, uint64_t *pa);
```

**Driver:** 7-access pattern through `translate`; print `hits/misses`. IOMMU ctx demonstrating (i) L1 missing, (ii) L2 missing, (iii) RO write fault.

**Interview prompts:** TLB coverage = entries × page size; the 4KB→2MB lever. TLB shootdown via IPI; ARM hardware TLBI broadcast vs x86. Why does the device need a separate IOMMU when the host has an MMU? ATS+PRI — what does it buy and what does it cost?

---

# Day 20 — DMA, doorbells, ordering, barriers

## D20 — SQ ring + doorbell harness (`d20_sq_doorbell`)

**Problem statement.** Producer side of a generic submission-queue / doorbell pattern. Build a 16-entry ring, a `volatile` MMIO doorbell stand-in, a `sq_submit(cmd, arg)` that fills the descriptor, issues a memory barrier, advances the tail, writes the doorbell. Then `device_consume_one()` simulates the device picking up the next entry from `sq_head`.

**Implement:**
```c
typedef struct { uint32_t cmd; uint32_t arg; } sqe_t;
int  sq_submit(uint32_t cmd, uint32_t arg);   /* 0 ok, -1 full */
void device_consume_one(void);

#define COMPILER_BARRIER() __asm__ volatile("" ::: "memory")
#define MEMORY_BARRIER()   __sync_synchronize()
#define CACHE_CLEAN(p, n)  /* stub */
```

**Driver:** `sq_submit(0x01, 0xAAAA)`, `sq_submit(0x02, 0xBBBB)`, then `device_consume_one()` × 3 (third = empty). Print head/tail movement.

**Interview prompts:** Why descriptor → barrier → doorbell, *always* in that order? Why is the failure intermittent ("works at low rate, fails at line rate")? `volatile` vs `dmb st` vs cache clean — what does each enforce? ARM `DMB ISH` vs `OSH` vs `SY` — when does each apply? IO-coherent vs non-coherent DMA — what changes in the driver?

---

# Day 21 — Pipeline + OoO microarch

## D21 — Pipeline simulator + 2-bit branch predictor + license-level model (`d21_pipeline_bp`)

**Problem statement.** Three small models you can talk through in 60 seconds.

(a) **2-bit saturating branch predictor.** State `bp ∈ [0,3]`. Predict NT if `bp < 2`, T if `bp ≥ 2`. Update saturates: `bp++` on taken, `bp--` on not-taken. Score against a fixed branch stream and report accuracy.

(b) **License-level effective frequency.** Given fractions of time spent in scalar / AVX2 / AVX-512 plus a base GHz, return effective frequency where AVX2 = 0.90×, AVX-512 = 0.75× base.

(c) **5-stage pipeline diagram.** Print `IF/ID/EX/ME/WB` for 4 instructions with one load-use stall after the first instruction (a 1-cycle bubble before instruction 2).

**Implement:**
```c
bool   predict(void);
void   update(bool taken);
double effective_freq_ghz(double scalar_pct, double avx2_pct, double avx512_pct, double base_ghz);
void   pipeline_demo(void);
```

**Driver:** Stream `{1,1,1,0,1,1,0,1,1,1,0,0,1}` → print BP accuracy. `(1.0, 0, 0, 4.0)` → 4.00 GHz; `(0.9, 0, 0.1, 4.0)` → ~3.90 (note net loss vs pure scalar). `pipeline_demo()` prints the 4-row diagram.

**Interview prompts:** OoO vs in-order; what extracts ILP. ROB / RS / rename — what each contributes. AVX-512 license-level pessimisation; when sprinkling AVX-512 is a net loss. HW prefetcher disable (`MSR 0x1A4`) as triage when silicon A passes / B regresses.

---

# Day 22 — Memory consistency + atomics

## D22 — Spinlock + SPSC ring with acq/rel ordering (`d22_spsc_acqrel`)

**Problem statement.** Two lock-free / lock-light primitives end-to-end.

(a) **Spinlock** — test-and-test-and-set. `spin_lock` busy-loads with `memory_order_relaxed`, then tries `compare_exchange_weak` with `memory_order_acquire`. `spin_unlock` does `memory_order_release`. Drive with two pthreads each bumping a shared `counter` 100 000 times; assert final value `== 2 * 100000`.

(b) **SPSC ring** — `R = 1024` slots. `head` and `tail` MUST sit on separate 64-byte cache lines (`alignas(64)`). Push: load tail relaxed, head acquire, write slot, store tail release. Pop: mirror. Drive: push 5 squares, pop them all, print.

**Implement:**
```c
typedef _Atomic int spinlock_t;
void spin_lock(spinlock_t *l);
void spin_unlock(spinlock_t *l);

typedef struct {
    int buf[R];
    alignas(64) _Atomic uint32_t head;
    alignas(64) _Atomic uint32_t tail;
} spsc_t;
bool spsc_push(spsc_t *r, int v);
bool spsc_pop (spsc_t *r, int *v);
```

**Driver:** Two-thread counter test → expect `200000`. SPSC push 5 squares (`0,1,4,9,16`), drain, print.

**Interview prompts:** TSO vs weak; why ARM needs `dmb ish` between producer data store and tail update. Pick weakest correct order; relaxed for counters, acq/rel for locks and producer-consumer flags, seq_cst rarely. Why SPSC needs no CAS; what changes at MPMC. ABA problem; defenses (tagged pointer, hazard pointer, RCU). Cache-line padding between `head`/`tail` is the difference between toy and shipped code.

---

# Day 23 — CPU bring-up + IP functional val

## D23 — CPU bring-up 9-stage ladder simulator (`d23_cpu_bringup_ladder`)

**Problem statement.** Model the canonical CPU IP bring-up ladder as a small state machine. Each stage has a `ready` flag and an ordered list of prerequisites; advancing requires the previous stage to be ready. Validation checks include illegal-skip detection, retry-on-fail, and a final "boot reached" assertion.

**Stages (in order):**
1. `RESET_DEASSERTED` — POR drops, scan chains clear.
2. `CLOCKS_STABLE` — PLL locked, refclk gated through.
3. `RAMS_BIST` — internal SRAM BIST passes (fuses programmed).
4. `MMU_ENABLED` — TTBR/CR3 written, walker active.
5. `CACHE_ENABLED` — L1I/L1D/L2 enabled in correct order.
6. `INT_CONTROLLER_UP` — GIC/APIC dispatch online.
7. `TIMERS_ONLINE` — generic timer / TSC running.
8. `SMP_RELEASED` — secondary cores released from holding pen (PSCI `CPU_ON` / x86 SIPI).
9. `BOOT_REACHED` — first user-mode instruction retired.

**Implement:**
```c
typedef enum {
    STG_RESET_DEASSERTED, STG_CLOCKS_STABLE, STG_RAMS_BIST,
    STG_MMU_ENABLED, STG_CACHE_ENABLED, STG_INT_CONTROLLER_UP,
    STG_TIMERS_ONLINE, STG_SMP_RELEASED, STG_BOOT_REACHED, STG_NUM
} stage_t;

typedef struct { bool ready[STG_NUM]; int retries[STG_NUM]; } bringup_t;

bool advance(bringup_t *b, stage_t next);   /* returns false on illegal skip */
bool inject_fail(bringup_t *b, stage_t at, int max_retries);
void report(const bringup_t *b);
```

**Driver:**
- Walk all 9 stages in order → `BOOT_REACHED`.
- Try `STG_RESET_DEASSERTED → STG_MMU_ENABLED` directly → `ILLEGAL_SKIP: clocks not stable, RAMs not BIST'd`.
- Inject one failure at `STG_RAMS_BIST` with `max_retries=2` → recovers and continues.

**Interview prompts:** Why this exact stage order — what breaks if you flip MMU and cache? What does an "IP functional val" team own at each stage (vplan, coverage, checkers)? Three coverage flavors (code, functional, assertion) and which one catches a missed stage. The 7-step bug-cost ladder (RTL sim → emul → FPGA → silicon → field) — where each catch costs.

---

# Day 24 — Verification Planning + Coverage

## D24 — Coverage tracker + vplan (`d24_vplan_coverage`)

**Problem statement.** Model the coverage-driven loop: define coverpoints with bins and a cross, feed randomized stimulus, report coverage and the **uncovered holes**, then close a hole with a directed test.

**Implement:**
```c
typedef struct { const char *name; int nbins; int *hits; } coverpoint_t;
void cover_sample(coverpoint_t *cp, int bin);
double cover_pct(const coverpoint_t *cp);          /* fraction of bins hit >=1 */
int    cover_holes(const coverpoint_t *cp, int *out);  /* list unhit bins */
/* a cross of two coverpoints */
double cross_pct(const coverpoint_t *a, const coverpoint_t *b, const int *seen);
typedef struct { const char *spec_ref, *test, *coverpoint; int covered; } vrow_t;
int    vplan_audit(const vrow_t *rows, int n, const coverpoint_t *cps);  /* fail if 'covered' but 0 hits */
```

**Driver / tests:**
- Feed random samples; show coverage climbing and a stuck **hole** bin; add a directed sample that closes it → 100%.
- `vplan_audit` flags a row marked covered whose coverpoint has zero hits (the checker-less-test smell).

**Interview prompts:** Recite the 8-step test-plan scaffold. Why is 100% code coverage insufficient? Hole vs legitimate exclusion? Directed vs CRV vs formal for three goals.

---

# Day 25 — Testbench Architecture + Assertions + Debug

## D25 — Scoreboard + constrained-random (`d25_scoreboard_crv`)

**Problem statement.** Build a **self-checking** testbench around a DUT model (reuse the D14 FIFO or the D5 ALU): a reference model, a constrained-random generator with a logged seed, a scoreboard that flags the **first** diverging transaction, and an error-injection mode.

**Implement:**
```c
typedef struct { uint32_t a, b; int op; } txn_t;
void     gen_txn(txn_t *t, unsigned *seed);        /* constrained: legal op/range only */
uint32_t ref_model(const txn_t *t);                /* golden expected result */
int      scoreboard(const txn_t *t, uint32_t dut_out, int *first_fail_idx, int idx);
int      assert_invariants(const txn_t *t, uint32_t dut_out);  /* e.g. no overflow, in-range */
void     inject_error(txn_t *t);                   /* force an illegal input */
```

**Driver / tests:**
- Run N constrained-random transactions through DUT + reference; scoreboard must pass and the **seed is printed** for replay.
- Introduce a deliberate DUT bug; assert the scoreboard reports the **first** diverging index, not a later one.
- Error-injection mode: illegal inputs trigger the flag/handling, never silent corruption.

**Interview prompts:** Why self-checking? What does an assertion catch that a scoreboard doesn't (and vice versa)? Functional vs error tests for this DUT? Narrate a first-divergence debug trace.

---

# Day 26 — DVFS / P-states / PVT

## D26 — DVFS table lookup with thermal cap + IPC counter (`d26_dvfs`)

**Problem statement.** Build a DVFS controller. Given a 5-entry P-state table (P4..P0 from `(400 MHz, 750 mV)` to `(2400 MHz, 1150 mV)`), implement `select_opp(target_mhz, tj_c)` that picks the lowest OPP meeting target frequency, **capped to ≤1200 MHz when `tj_c >= 90`** (thermal throttle). Then `apply_opp(cur, next)` that prints the correct sequencing — **V before F when ramping up, F before V when ramping down**.

Plus a tiny `pmu_t { insns, cycles }` and `ipc()` helper.

**Implement:**
```c
typedef struct { uint32_t freq_mhz; uint32_t volt_mv; } opp_t;
const opp_t *select_opp(uint32_t target_mhz, int tj_c);
void         apply_opp(const opp_t *cur, const opp_t *next);

typedef struct { uint64_t insns, cycles; } pmu_t;
double ipc(const pmu_t *p);
```

**Driver:** OPP for `(400, 50)`, then `(2000, 50)` → ramp-up sequence printed. `(2400, 95)` → thermal cap to 1200 MHz, ramp-down sequence printed. IPC for `insns=15e9, cycles=12.34e9` and flag if `< 1.0`.

**Interview prompts:** Derive $P_{dyn} = \tfrac{1}{2}CV^2F$; explain V² dominance. Why V-before-F up, F-before-V down? Setup-time slack argument. Why discrete P-states vs continuous? Validation-matrix argument. Why governors exist (HW sees load, not intent). AVS / PVT — what does the real-time sensor close that fuses can't.

---

# Day 27 — C-states / PkgC / S0ix

## D27 — C-state request + PkgC6 PCU gate model (`d27_pkgc6_gate`)

**Problem statement.** Model the PCU's PkgC6 gating. 4 cores (`cores[NCORES]`), each with `cstate_t cs` and a `work_pending` flag. `request_core_cs(id, target)` refuses any state deeper than C1 if work is pending. `try_pkg_c6()` checks **all** cores in C6 *and* every uncore IP (`llc_idle`, `mc_idle`, `pcie_in_l1_2`, `display_idle`, `intc_idle`) reports idle. If blocked, print which IP blocks. Default `pcie_in_l1_2 = false` so the first attempt fails.

**Implement:**
```c
typedef enum { C0=0, C1=1, C1E=2, C6=6 } cstate_t;
typedef struct { cstate_t cs; bool work_pending; } core_t;
typedef struct { bool llc_idle, mc_idle, pcie_in_l1_2, display_idle, intc_idle; } uncore_t;

bool request_core_cs(int id, cstate_t target);
bool try_pkg_c6(void);
const char *blocker(void);
```

**Driver:** All 4 cores → request C6. `try_pkg_c6()` → "BLOCKED: PCIe link not in L1.2". Set `un.pcie_in_l1_2 = true`, retry → "PkgC6 ENTERED". *(This blocker pattern is the seed of the Day 37 PCIe×PM intersection exercise.)*

**Interview prompts:** C1 vs C1E vs C6 vs C10 — clock-gated vs power-gated, retention SRAM, wake latency. Why does PkgC6 need *every* uncore IP idle? Shared-rail and snoop-traffic arguments. Break-even residency $T_r > (E_{\text{in}} + E_{\text{out}}) / \Delta P$. L1.2 dependency for PkgC6; LTR/CLKREQ#/ASPM bring-up checklist.

---

# Day 28 — Clock + power gating + UPF

## D28 — 3-domain UPF power model with SAVE/ISO/RESTORE asserts (`d28_upf_domain_model`)

**Problem statement.** Model three power domains (`ALWAYS_ON`, `CPU_CLUSTER`, `GPU`) with proper UPF-style sequencing. Each domain has `power`, `iso`, `retention_valid` flags. Power-down sequence: assert `iso` → assert `clamp` → save retention flops → drop power. Power-up sequence: restore power → wait for stable → restore from retention → de-assert `clamp` → de-assert `iso`. The model includes **assertions** that fail loudly if any step is skipped.

**Required violations to demonstrate:**
- Dropping power before `iso` is asserted → `ASSERT FAIL: outputs floating, downstream X-propagation`.
- De-asserting `iso` before power is back → `ASSERT FAIL: spurious enable into clamp`.
- Skipping retention save → `ASSERT FAIL: state lost across power cycle`.

**Implement:**
```c
typedef struct {
    const char *name;
    bool powered, iso_asserted, retention_valid;
    uint32_t saved_state;   /* stand-in for retained flop content */
} pdomain_t;

bool power_down(pdomain_t *d);     /* enforces SAVE → ISO → CLAMP → POWER_OFF */
bool power_up  (pdomain_t *d);     /* enforces POWER_ON → RESTORE → CLAMP_OFF → ISO_OFF */
bool assert_safe(const pdomain_t *d, const char *step);
```

**Driver:**
- Normal cycle on `GPU`: down then up; print every step and final state == saved state.
- Inject `bad_power_down()` that drops power before `iso` → assertion fires.
- Show the **ICG** notion as a one-line stand-in: only the CPU_CLUSTER domain's free-running clock toggles when not gated; assert "0 toggles when gate de-asserted".

**Interview prompts:** ICG cells — why latch-and-AND instead of plain AND. Header vs footer FET tradeoffs (Vt, leakage, layout). Retention flop = master-slave with always-on slave; why not just SRAM. Isolation cell purpose, where in the boundary. Level shifters between domain voltages. UPF (IEEE 1801) — what does the declaration give the synthesis + LP-sim flow that RTL can't.

---

# Day 29 — Thermal, perf counters, multi-IP

## D29 — Tj model + PROCHOT + RAPL delta + TMA bucket (`d29_tj_rapl_tma`)

**Problem statement.** Three composing models for a perf/thermal validation harness.

(a) **Thermal step.** $T_j = T_a + P \cdot \theta_{JA}$ with first-order RC smoothing (tau=5s). On each `dt`, push `tj` toward steady state. Assert PROCHOT when `tj >= prochot`; clear when `tj < prochot - 5` (5°C hysteresis). When throttled, the step uses 75 W instead of 125 W (caller decides, model just observes).

(b) **RAPL delta → watts.** `rapl_watts(e0, e1, unit_uj, dt)` converts a counter delta + LSB-µJ + duration to watts.

(c) **Top-down dominant bucket.** Given fractions `frontend_bound, bad_spec, retiring, mem_bound, core_bound`, return the name of the largest.

**Implement:**
```c
typedef struct { double tj, ta, theta, prochot; bool throttled; } thermal_t;
void thermal_step(thermal_t *m, double power, double dt);

double rapl_watts(uint64_t e0, uint64_t e1, uint64_t unit_uj, double dt);

typedef struct { double frontend_bound, bad_spec, retiring, mem_bound, core_bound; } tma_t;
const char *tma_dominant(const tma_t *t);
```

**Driver:** 30-second sim with ambient 25, theta 0.3, prochot 95, power 125 W (throttled → 75) — print PROCHOT enter/clear events, final `tj` and `throttled`. `rapl_watts(0, 65000000, 1, 1.0)` → 65.0 W. TMA `(0.15, 0.05, 0.25, 0.45, 0.10)` → "Backend Memory".

**Interview prompts:** $T_j = T_a + P\theta_{JA}$ as lumped RC; when the model breaks (transient PROCHOT, hotspot). Why RAPL underestimates wall by 20–30%; what closes each gap. Top-down's 4 buckets; what they isolate. HW prefetcher hides missing software prefetch — why disabling HW PF is triage, not fix. Leakage-temperature runaway and why throttle is the *negative feedback* that breaks the loop.

---

# Day 30 — Pre-Si PM validation

## D30 — PM coverage tracker (state × transition × residency) (`d30_pm_coverage_tracker`)

**Problem statement.** Build the **3-axis coverage tracker** described in the Day 30 reading. As the model receives a stream of `(core_id, from_state, to_state, dwell_us)` events, it must maintain:

(a) **State coverage** — which (`C0`, `C1`, `C1E`, `C6`, `C10`) states each core has been observed in.
(b) **Transition coverage** — which `from → to` pairs have fired. Use the legal-transition table from Day 27.
(c) **Residency-bin coverage** — for each state, did the core hit `short` (<10 µs), `medium` (10-1000 µs), `long` (>1 ms) dwell bins.

At end of run, print a coverage report and flag any axis below 80% coverage as a regression-blocker. Detect **two illegal transitions** in the input stream and emit `SVA_FAIL: illegal transition C0 → C6 (must pass through C1 first)`.

**Implement:**
```c
typedef enum { C0=0, C1, C1E, C6, C10, CSTATE_NUM } cstate_t;
typedef enum { BIN_SHORT=0, BIN_MED, BIN_LONG, BIN_NUM } resbin_t;

typedef struct {
    bool state_seen[CSTATE_NUM];
    bool trans_seen[CSTATE_NUM][CSTATE_NUM];
    bool resbin_seen[CSTATE_NUM][BIN_NUM];
    int sva_fails;
} coverage_t;

void record_event(coverage_t *c, int core, cstate_t from, cstate_t to, double dwell_us);
void report_coverage(const coverage_t *c);
```

**Driver:** Generate a 50-event stream covering 4 of 5 states, 12 of legal transitions, 2 of 3 bins for C6, all 3 for C0. Plant two illegal transitions. Expect: state 80%, transition ~60% (flag), residency mixed. Print the regression-blocker summary.

**Interview prompts:** Why the 3-axis frame and not just state coverage? The 6 mandatory PM SVAs (SAVE-before-power-down, ISO-before-clamp-release, illegal C-state transition, residency-min, wake-latency-max, no-wake-during-save). Perspec for cross-IP PM scenarios. LP-sim (VC-LP / Conformal-LP) versus functional sim. PCU opcode trace as the golden-ref for the post-Si side (Day 31).

---

# Day 31 — Post-Si PM validation + ADR/C6 bugs

## D31a — Wrap-safe residency-counter delta parser (`d31_residency_delta`)

**Problem statement.** Build the post-Si tool that reads C-state residency MSRs at two timestamps and computes the percent of wall-clock time spent in each state, *correctly handling counter wrap*. The MSR is a 64-bit free-running TSC-counted residency register; reading at `t0` and `t1` (with `t1 - t0` in TSC ticks) gives `(end - start) mod 2^64` ticks resident. Compute `pct = 100.0 * residency_delta / wall_tsc_delta` for each state and print a `turbostat`-style row.

**Required edge cases:**
- Counter wrap: `start = 0xFFFF_FFFF_FFFF_FF00`, `end = 0x100`. Delta must be `0x200` (not negative).
- Residency exceeds wall (impossible) → flag as `SANITY: residency > wall, MSR mis-programmed`.
- All cores aggregated: per-core columns plus package total.

**Implement:**
```c
typedef struct {
    uint64_t cc3, cc6, cc7;       /* core-C residency */
    uint64_t pc2, pc6, pc10;      /* package-C residency */
    uint64_t tsc;                 /* free-running ref */
} msr_snap_t;

uint64_t wrap_delta(uint64_t end, uint64_t start);
void     print_row(const char *label, const msr_snap_t *t0, const msr_snap_t *t1);
```

**Driver:** Two snapshots 1 second apart, plant a counter-wrap on `cc6`, plant an over-100% on `pc6`. Expect: correct CC6 percentage despite wrap, sanity warning on PC6.

**Interview prompts:** The 8-stage post-Si PM bring-up ladder (silicon → BIOS PM → ACPI tables → kernel governor → user workload → measurement → MSR sweep → bug isolation). turbostat / socwatch / `rdmsr` / `wrmsr` / RAPL / Quarch / Lauterbach — what each tool sees that the others don't. The 7-step debug framework. The **ADR opcode-ordering bug** and the **Core C6 wake-loss bug** — be ready to tell each as a 90-sec story.

## D17b — *(optional)* ADR opcode-sequence simulator (`d31_adr`)

**Problem statement.** Model the PCU's ADR (Asynchronous DRAM Refresh) flow as an opcode sequence. Two versions:

(a) **Buggy** — issues `WPQ_FLUSH` without first waiting for `IO_QUIESCE_ACK`. Model detects this and prints `BUG: WPQ_FLUSH issued before IO_QUIESCE_ACK -> WPQ refills, hang`.

(b) **Fixed** — strict ordering: `IO_QUIESCE` + ack → `FABRIC_QUIESCE` → optional `LLC_WRITEBACK` (eADR) → `WPQ_FLUSH` + ack → `DDR_SELF_REFRESH` → `NVDIMM_SAVE`. Report `in_self_refresh = true` on success.

**Implement:**
```c
typedef enum {
    OP_IO_QUIESCE, OP_IO_QUIESCE_ACK, OP_FABRIC_QUIESCE, OP_LLC_WRITEBACK,
    OP_WPQ_FLUSH, OP_WPQ_FLUSH_ACK, OP_DDR_SELF_REFRESH, OP_NVDIMM_SAVE, OP_DONE
} adr_op_t;
typedef struct { bool io_quiesced, fabric_quiesced, wpq_drained, in_self_refresh, eadr; } adr_state_t;
int adr_run_buggy(adr_state_t *s);
int adr_run_fixed(adr_state_t *s);
```

**Driver:** Buggy → expect failure print. Fixed → expect `rc=0`, `in_self_refresh=1`.

**Interview prompts:** Cambria-style ADR FW bug (`WPQ_FLUSH` before `IO_QUIESCE_ACK`); Zebu opcode trace; Perspec assertion gating regression. Core C6 wake-lost CFC FIFO race; Lauterbach trigger `wake_in==1 && cfc_init_busy==1`; fix is hold latch in always-on until `init_done && drain_done`. ADR hold-up budget physics (10-30 ms). Why PCU is firmware, not RTL.

---

# Day 32 — AMBA AXI & CHI

## D32 — AXI model (`d32_axi_model`)

**Problem statement.** Model the AXI handshake, ordering model, burst rules, and arbiter so the protocol-validation hooks are concrete.

**Implement:**
```c
/* VALID/READY: transfer when both high; detect VALID-waits-on-READY deadlock */
int axi_xfer(int src_has_data, int valid_waits_on_ready, int dst_ready, int *deadlock);
/* scoreboard: same-ID responses must stay ordered; different-ID may reorder */
int axi_order_check(const int *resp_id, const int *resp_seq, int n);
/* INCR/WRAP burst address gen; flag a 4KB-boundary crossing */
int axi_burst(uint64_t base, int len, int size, int wrap, uint64_t *addrs, int *cross4k);
/* round-robin arbiter with a starvation check */
int rr_arbiter(const int *req, int n, int *last_grant);
```

**Driver / tests:** show a deadlock when VALID waits on READY; `axi_order_check` passes same-ID-ordered traffic and flags a same-ID reorder; an INCR burst that crosses 4KB is flagged; the arbiter serves every requester within one rotation (no starvation).

**Interview prompts:** Why must VALID not wait on READY? Same-ID vs cross-ID ordering? How is CHI just MESI at interconnect scale?

---

# Day 33 — PCIe fundamentals + LTSSM

## D33 — LTSSM walker (`d33_ltssm_walk`)

**Problem statement.** Define the 10 LTSSM states (Detect, Polling, Config, L0, Recovery, L0s, L1, L1.1, L1.2, L2). Build an allowed-transition table. `go(link, to)` enforces legality, prints transitions, counts `recoveries` whenever entering Recovery. Add a `bar_size_from_readback(uint32_t rb)` helper as an aside: write `0xFFFFFFFF` to BAR, read back, mask type bits, invert and add 1 to get size — Day 34 will reuse it.

**Implement:**
```c
typedef enum { LTSSM_DETECT, LTSSM_POLLING, LTSSM_CONFIG, LTSSM_L0,
               LTSSM_RECOVERY, LTSSM_L0S, LTSSM_L1, LTSSM_L1_1,
               LTSSM_L1_2, LTSSM_L2, LTSSM_NUM } ltssm_t;
typedef struct { ltssm_t st; unsigned recoveries; } link_t;
bool     go(link_t *l, ltssm_t to);
uint64_t bar_size_from_readback(uint32_t rb);
```

**Driver:** Walk `Detect → Polling → Config → L0 → Recovery → L0 → L1 → L1.2 → Recovery → L0`. One illegal `Detect → L0` → `ILLEGAL`. `bar_size_from_readback(0xFFF00004)` → `0x100000`.

**Interview prompts:** 3 PCIe layers (PHY/DLL/TL); what each isolates. Posted vs non-posted; why doorbell-as-MWr won. L1 vs L1.1 vs L1.2; PkgC6 dependency on L1.2. Credit-based flow control vs ACK/NAK pacing. Gen6 FLIT and L0p — what changed.

---

# Day 34 — PCIe enumeration, config, BARs, MSI/MSI-X

## D34 — Enumeration walker model (`d34_enum_walker`)

**Problem statement.** Model the BIOS/UEFI depth-first enumeration walk. Given a fake `(bus, dev, fn)` tree, walk every function, read its config header type, recurse on bridges (type-1), program BAR sizes via the write-all-1s trick, build a tree of discovered devices. Track:
- bridge windows (mem, prefetch, IO) and the **must-be-power-of-2-and-aligned** rule.
- MSI vs MSI-X capability presence (walk the capability chain).
- bus-number assignment with the **secondary/subordinate** range.

**Implement:**
```c
typedef struct { uint16_t vid, did; uint8_t header_type;
                 uint32_t bar[6]; bool msi, msix; } func_t;
typedef struct { uint8_t bus, dev, fn; func_t f; } node_t;
typedef struct { node_t *nodes; int count; } tree_t;

void enumerate(uint8_t bus, tree_t *out);
uint64_t bar_size(uint32_t bar_readback);       /* reuse Day 33 helper */
void program_bridge_window(node_t *bridge, uint64_t mem_base);
```

**Driver:** Fake 3-bus topology (root → bridge → 2 endpoints), one endpoint with MSI-X. Run `enumerate(0, &t)`. Expect: 4 functions discovered, bridge windows computed, MSI-X table BIR/offset printed.

**Interview prompts:** Why depth-first and not breadth-first? Header types 0/1/2 — which is endpoint, which is bridge. BAR sizing trick: why write-all-1s works (decoder ties low bits). MSI vs MSI-X scaling argument. Capability chain — linked list at offset 0x34, why this format. Bridge window rules — alignment, prefetch vs non-prefetch.

---

# Day 35 — PCIe power management (ASPM / L1.2)

## D35 — ASPM / L1.2 FSM with CLKREQ# ordering asserts (`d35_aspm_l12_fsm`)

**Problem statement.** Build the ASPM state machine focused on **L1.2 entry and exit**, with strict side-band timing assertions. Track:
- link state (`L0`, `L0s`, `L1`, `L1.1`, `L1.2`)
- `CLKREQ#` (active-low, so model as `clkreq_asserted` boolean)
- `refclk_on`, `vcm_held` flags

**Required ordering** (any violation = `ASSERT FAIL`):
- **Entry to L1.2**: enter L1 → enter L1.1 → de-assert CLKREQ# → wait T_POWER_OFF (model with a tick counter) → drop refclk and Vcm → state = L1.2.
- **Exit from L1.2**: re-assert CLKREQ# → wait T_CLKREQ_TO_REFCLK → refclk valid → Vcm valid → wait T_L1.2.EXIT (typically ~10 µs) → state = L0.

Plant violations:
- Drop refclk before CLKREQ# de-asserts → `ASSERT FAIL: CLKREQ# must precede refclk gate`.
- Send TLP before L1.2 exit completes → `ASSERT FAIL: TLP during L1.2 exit`.

**Implement:**
```c
typedef enum { ASPM_L0, ASPM_L0S, ASPM_L1, ASPM_L1_1, ASPM_L1_2 } aspm_t;
typedef struct {
    aspm_t st;
    bool clkreq_asserted;   /* true == active-low asserted */
    bool refclk_on, vcm_held;
    int  tick;
} aspm_link_t;

bool enter_l12(aspm_link_t *l);
bool exit_l12 (aspm_link_t *l);
bool send_tlp (aspm_link_t *l);   /* must be in L0 */
```

**Driver:** Clean entry+exit cycle. Then plant violation: drop refclk before CLKREQ#. Then plant: TLP during exit window.

**Interview prompts:** ASPM ladder L0/L0s/L1/L1.1/L1.2 — what each level gates (link, refclk, Vcm, CLKREQ#). Why L1.2 is the **deepest** and why platform PkgC10 depends on it. LTR — what does the EP advertise and what does the RC do with it. D-states vs L-states — orthogonal axes. PME path on exit. The two timing constants `T_POWER_OFF` and `T_L1.2.EXIT` — why they exist physically.

---

# Day 36 — PCIe bring-up + functional val

## D36 — LTSSM stuck-state classifier (`d36_ltssm_stuck_classifier`)

**Problem statement.** Build the post-Si tool that takes an LTSSM state snapshot + symptom set and classifies the likely root cause. Input is `{ltssm_t stuck_in, bool refclk_present, bool perst_released, int lanes_detected, int target_lanes, gen_t target_gen, gen_t actual_gen, bool eq_phase_failed}`. Output one of:

- `STUCK_DETECT` — refclk missing OR PERST# stuck asserted OR receiver detect failing.
- `STUCK_POLLING` — TS1/TS2 not exchanged; check lane reversal, polarity inversion, ref-clock spread-spectrum mismatch.
- `STUCK_CONFIG` — lane count mismatch (target 16, detected 8 → bad solder or broken lane).
- `STUCK_RECOVERY` — equalization Phase 6/3 failed, signal-integrity issue.
- `STUCK_SPEED` — reached L0 but at lower gen than target (EQ Phase 0 floor only).
- `STUCK_UNKNOWN`.

For each, print the next debug action (`scope on REFCLK`, `Lecroy analyser TS1 capture`, `re-run EQ with relaxed preset`, etc.).

**Implement:**
```c
typedef enum { STUCK_DETECT, STUCK_POLLING, STUCK_CONFIG,
               STUCK_RECOVERY, STUCK_SPEED, STUCK_UNKNOWN } stuck_class_t;
typedef enum { GEN1=1, GEN2, GEN3, GEN4, GEN5, GEN6 } gen_t;

typedef struct {
    ltssm_t stuck_in;
    bool refclk_present, perst_released, eq_phase_failed;
    int  lanes_detected, target_lanes;
    gen_t target_gen, actual_gen;
} symptom_t;

stuck_class_t classify(const symptom_t *s);
const char *next_action(stuck_class_t c);
```

**Driver:** 5 canned symptom sets — one per class except UNKNOWN. Expect 5 correct classifications and 5 next-action lines.

**Interview prompts:** The 10-step bring-up ladder (PERST# → refclk → Detect → ... → L0 @ target gen). LTSSM stuck-state diagnostic table — what symptom maps to which state. Gen3+ equalisation 4 phases (Phase 0 floor, Phase 4 TX preset, Phase 6 RX tuning, Phase 7 final). AER categories (correctable, non-fatal, fatal) and what each tells you. Compliance test mode (loopback master/slave). Lecroy / Teledyne PETrainer / Keysight scope — what each catches.

---

# Day 37 — PCIe × PM intersection

## D37 — PkgC blocker arbiter (`d37_pkgc_blocker_arbiter`)

**Problem statement.** This is the post-Si tool for the *most-asked debug question* of the role: "platform never enters PkgC10, what's blocking it?" Build an arbiter that aggregates votes from every IP that participates in deep-idle gating and prints the **dependency chain** of blockers, not just the leaf cause.

Inputs:
- per-core C-state.
- each PCIe link's ASPM state (must be L1.2 for PkgC10).
- LTR values from each EP (a too-aggressive LTR forces RC to stay shallow).
- USB host controller idle vote.
- Display engine idle vote.
- Memory controller self-refresh vote.

Output: `PkgC10 BLOCKED by: PCIe link 03:00.0 (state L1, EP LTR=10 µs forces shallow) → Storage NVMe EP keep-alive → driver hint stuck`.

**Implement:**
```c
typedef enum { PKG_C0=0, PKG_C2, PKG_C6, PKG_C10 } pkg_state_t;

typedef struct {
    const char *bdf;
    aspm_t state;
    int    ltr_us;   /* lower = "wake me sooner", forces shallower PkgC */
} pcie_vote_t;

typedef struct {
    int n_cores; cstate_t core_cs[8];
    int n_links; pcie_vote_t links[16];
    bool usb_idle, display_idle, mc_self_refresh;
} platform_t;

pkg_state_t arbitrate(const platform_t *p, char *why, size_t whysz);
```

**Driver:** Three scenarios:
1. Everything idle, all links in L1.2, LTRs ≥ 1 ms → `PKG_C10`.
2. One link in L1 with LTR=10 µs → `PKG_C6` with blocker explanation.
3. USB host not idle → `PKG_C2` with blocker explanation.

**Interview prompts:** The PkgC6 / PkgC10 blocker chain — what each leaf vote means. L1.2 ↔ PkgC10 handshake (10-step: kernel → governor → PMC → RC → EP → CLKREQ# → ...). Deadlock farm: cyclic LTR negotiation, stuck CLKREQ#, half-gated rail. The retained **ADR ↔ PkgC** recap (Day 31 bug shows up here as cross-IP). The **L0p mid-FLIT** Gen6 bug — what it costs at PM intersection. 10-step debug playbook for "no PkgC10".

---

# Day 39 — Logic Puzzles + Estimation (optional)

## D39 — Puzzle simulators (`d39_puzzle_sims`)

**Problem statement.** *(Optional — this day is reasoning-first.)* Verify two puzzles by simulation so the result is felt, not just asserted.

**Implement:**
```c
/* 100 prisoners: each opens half the boxes. Returns 1 if ALL succeed. */
int prisoners_random(int n, unsigned *seed);     /* random opening  -> ~0% */
int prisoners_cycle (int n, unsigned *seed);     /* cycle-following -> ~31% */
double monte_carlo(int (*trial)(int,unsigned*), int n, int iters, unsigned seed);
/* coin game: pile of N, take 1..k, last to take wins. Returns 1 if first player wins under N%(k+1). */
int coin_game_optimal(int N, int k);
```

**Driver / tests:**
- `monte_carlo(prisoners_cycle, 100, 100000, seed)` ≈ **0.31** (`1 - ln 2`); `prisoners_random` ≈ 0.
- `coin_game_optimal(100,10)==1` (first player wins by taking 1 → leaves 99 = 9×11); `coin_game_optimal(99,10)==0`.

**Interview prompts:** Name the strategy before solving. The information-theoretic bound for a weighing puzzle. What tiny case sanity-checks the answer? Why does cycle-following beat random by ~10^29?

---

# Days 40–41 — narrative + final mock

No new code. **Day 40** (Intel-role narrative) and **Day 41** (final mock) point at the *content* you've built — re-run any 2–3 exercises cold (e.g. `d09_fsm_detect` / `d14_async_fifo` / `d24_vplan_coverage` / `d23_cpu_bringup_ladder`) as warm-ups.

---

## Reading map

Every day file lives in [../prep_plan/](../prep_plan/). Architect drills, bring-up flow, tools (pre-Si + post-Si), and bug anchors are in the day file — coding is in this file. Don't duplicate; cross-link.
