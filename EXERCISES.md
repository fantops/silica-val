# Coding — Single Exercise Document

> One folder, one document. All 25 days of coding live here. Systems-C only. Read the prompt, write the code in `exercises/dNN_<name>.c`, build, run, explain *why*. Only then peek at `solutions/dNN_<name>.c`.

This file is the coding companion to [prep_plan/README.md](../prep_plan/README.md). The plan is **25 days, no DSA, no LeetCode** — every exercise is a bit/CSR/FSM/PM/PCIe systems-C model tied to the day's reading.

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
| 4 | Cache lines / locality / AMAT | `d04_cache_sim` |
| 5 | Coherence / MESI / false sharing | `d05_mesi_sim` |
| 6 | Virtual memory / page walk | `d06_page_walk` |
| 7 | TLB / IOMMU | `d07_tlb_iommu` |
| 8 | DMA / doorbells / barriers | `d08_sq_doorbell` |
| 9 | Pipeline + OoO microarch | `d09_pipeline_bp` |
| 10 | Memory consistency + atomics | `d10_spsc_acqrel` |
| 11 | CPU bring-up + IP functional val | `d11_cpu_bringup_ladder` |
| 12 | DVFS / P-states / PVT | `d12_dvfs` |
| 13 | C-states / PkgC / S0ix | `d13_pkgc6_gate` |
| 14 | Clock + power gating + UPF | `d14_upf_domain_model` |
| 15 | Thermal / perf counters | `d15_tj_rapl_tma` |
| 16 | Pre-Si PM validation | `d16_pm_coverage_tracker` |
| 17 | Post-Si PM validation + ADR/C6 bugs | `d17_residency_delta` (+ `d17_adr` opcode model) |
| 18 | PCIe fundamentals + LTSSM | `d18_ltssm_walk` |
| 19 | PCIe enumeration / config / MSI | `d19_enum_walker` |
| 20 | PCIe power management (ASPM/L1.2) | `d20_aspm_l12_fsm` |
| 21 | PCIe bring-up + functional val | `d21_ltssm_stuck_classifier` |
| 22 | PCIe × PM intersection | `d22_pkgc_blocker_arbiter` |
| 23 | MSFT/NPU exit pointers (1 day) | — *(no new exercise; reuse `d06_page_walk` to anchor the QNN story)* |
| 24 | Final mock interview day | — *(re-run any 2-3 exercises cold as warm-up)* |

Each Day 1-22 entry below has one **Systems C exercise** with full problem statement, signatures, driver, and interview prompts. Architect drills and bug anchors live in the matching [`prep_plan/dayNN_*.md`](../prep_plan/) reading file (kept in one place to avoid duplication).

---

# Day 1 — C address fluency

## D01 — Register-poke harness (`d01_reg_poke`)

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

## D02 — Aligned allocator + container_of (`d02_aligned_alloc`)

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

## D03 — Device init harness with poll + RMW (`d03_dev_init`)

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

# Day 4 — Cache lines, locality, AMAT

## D04 — Address decode + cache simulator (`d04_cache_sim`)

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

# Day 5 — Coherence, MESI, false sharing

## D05 — MESI step-machine + false-sharing layout check (`d05_mesi_sim`)

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

# Day 6 — Virtual memory, page faults

## D06 — 4-level page walk (`d06_page_walk`)

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

**Interview prompts:** Why 4 levels and not flat? Why is page offset never translated? PFN vs physical address? Page fault vs TLB miss vs cache miss in 30 sec. *(This is also the anchor exercise for the Day 23 QNN page-fault bug story.)*

---

# Day 7 — TLB, huge pages, IOMMU/SMMU

## D07 — TLB lookup with FIFO + IOMMU translate (`d07_tlb_iommu`)

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

# Day 8 — DMA, doorbells, ordering, barriers

## D08 — SQ ring + doorbell harness (`d08_sq_doorbell`)

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

# Day 9 — Pipeline + OoO microarch

## D09 — Pipeline simulator + 2-bit branch predictor + license-level model (`d09_pipeline_bp`)

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

# Day 10 — Memory consistency + atomics

## D10 — Spinlock + SPSC ring with acq/rel ordering (`d10_spsc_acqrel`)

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

# Day 11 — CPU bring-up + IP functional val

## D11 — CPU bring-up 9-stage ladder simulator (`d11_cpu_bringup_ladder`)

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

# Day 12 — DVFS / P-states / PVT

## D12 — DVFS table lookup with thermal cap + IPC counter (`d12_dvfs`)

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

# Day 13 — C-states / PkgC / S0ix

## D13 — C-state request + PkgC6 PCU gate model (`d13_pkgc6_gate`)

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

**Driver:** All 4 cores → request C6. `try_pkg_c6()` → "BLOCKED: PCIe link not in L1.2". Set `un.pcie_in_l1_2 = true`, retry → "PkgC6 ENTERED". *(This blocker pattern is the seed of the Day 22 PCIe×PM intersection exercise.)*

**Interview prompts:** C1 vs C1E vs C6 vs C10 — clock-gated vs power-gated, retention SRAM, wake latency. Why does PkgC6 need *every* uncore IP idle? Shared-rail and snoop-traffic arguments. Break-even residency $T_r > (E_{\text{in}} + E_{\text{out}}) / \Delta P$. L1.2 dependency for PkgC6; LTR/CLKREQ#/ASPM bring-up checklist.

---

# Day 14 — Clock + power gating + UPF

## D14 — 3-domain UPF power model with SAVE/ISO/RESTORE asserts (`d14_upf_domain_model`)

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

# Day 15 — Thermal, perf counters, multi-IP

## D15 — Tj model + PROCHOT + RAPL delta + TMA bucket (`d15_tj_rapl_tma`)

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

# Day 16 — Pre-Si PM validation

## D16 — PM coverage tracker (state × transition × residency) (`d16_pm_coverage_tracker`)

**Problem statement.** Build the **3-axis coverage tracker** described in the Day 16 reading. As the model receives a stream of `(core_id, from_state, to_state, dwell_us)` events, it must maintain:

(a) **State coverage** — which (`C0`, `C1`, `C1E`, `C6`, `C10`) states each core has been observed in.
(b) **Transition coverage** — which `from → to` pairs have fired. Use the legal-transition table from Day 13.
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

**Interview prompts:** Why the 3-axis frame and not just state coverage? The 6 mandatory PM SVAs (SAVE-before-power-down, ISO-before-clamp-release, illegal C-state transition, residency-min, wake-latency-max, no-wake-during-save). Perspec for cross-IP PM scenarios. LP-sim (VC-LP / Conformal-LP) versus functional sim. PCU opcode trace as the golden-ref for the post-Si side (Day 17).

---

# Day 17 — Post-Si PM validation + ADR/C6 bugs

## D17a — Wrap-safe residency-counter delta parser (`d17_residency_delta`)

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

## D17b — *(optional)* ADR opcode-sequence simulator (`d17_adr`)

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

# Day 18 — PCIe fundamentals + LTSSM

## D18 — LTSSM walker (`d18_ltssm_walk`)

**Problem statement.** Define the 10 LTSSM states (Detect, Polling, Config, L0, Recovery, L0s, L1, L1.1, L1.2, L2). Build an allowed-transition table. `go(link, to)` enforces legality, prints transitions, counts `recoveries` whenever entering Recovery. Add a `bar_size_from_readback(uint32_t rb)` helper as an aside: write `0xFFFFFFFF` to BAR, read back, mask type bits, invert and add 1 to get size — Day 19 will reuse it.

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

# Day 19 — PCIe enumeration, config, BARs, MSI/MSI-X

## D19 — Enumeration walker model (`d19_enum_walker`)

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
uint64_t bar_size(uint32_t bar_readback);       /* reuse Day 18 helper */
void program_bridge_window(node_t *bridge, uint64_t mem_base);
```

**Driver:** Fake 3-bus topology (root → bridge → 2 endpoints), one endpoint with MSI-X. Run `enumerate(0, &t)`. Expect: 4 functions discovered, bridge windows computed, MSI-X table BIR/offset printed.

**Interview prompts:** Why depth-first and not breadth-first? Header types 0/1/2 — which is endpoint, which is bridge. BAR sizing trick: why write-all-1s works (decoder ties low bits). MSI vs MSI-X scaling argument. Capability chain — linked list at offset 0x34, why this format. Bridge window rules — alignment, prefetch vs non-prefetch.

---

# Day 20 — PCIe power management (ASPM / L1.2)

## D20 — ASPM / L1.2 FSM with CLKREQ# ordering asserts (`d20_aspm_l12_fsm`)

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

# Day 21 — PCIe bring-up + functional val

## D21 — LTSSM stuck-state classifier (`d21_ltssm_stuck_classifier`)

**Problem statement.** Build the post-Si tool that takes an LTSSM state snapshot + symptom set and classifies the likely root cause. Input is `{ltssm_t stuck_in, bool refclk_present, bool perst_released, int lanes_detected, int target_lanes, gen_t target_gen, gen_t actual_gen, bool eq_phase_failed}`. Output one of:

- `STUCK_DETECT` — refclk missing OR PERST# stuck asserted OR receiver detect failing.
- `STUCK_POLLING` — TS1/TS2 not exchanged; check lane reversal, polarity inversion, ref-clock spread-spectrum mismatch.
- `STUCK_CONFIG` — lane count mismatch (target 16, detected 8 → bad solder or broken lane).
- `STUCK_RECOVERY` — equalization Phase 2/3 failed, signal-integrity issue.
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

**Interview prompts:** The 10-step bring-up ladder (PERST# → refclk → Detect → ... → L0 @ target gen). LTSSM stuck-state diagnostic table — what symptom maps to which state. Gen3+ equalisation 4 phases (Phase 0 floor, Phase 1 TX preset, Phase 2 RX tuning, Phase 3 final). AER categories (correctable, non-fatal, fatal) and what each tells you. Compliance test mode (loopback master/slave). Lecroy / Teledyne PETrainer / Keysight scope — what each catches.

---

# Day 22 — PCIe × PM intersection

## D22 — PkgC blocker arbiter (`d22_pkgc_blocker_arbiter`)

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

**Interview prompts:** The PkgC6 / PkgC10 blocker chain — what each leaf vote means. L1.2 ↔ PkgC10 handshake (10-step: kernel → governor → PMC → RC → EP → CLKREQ# → ...). Deadlock farm: cyclic LTR negotiation, stuck CLKREQ#, half-gated rail. The retained **ADR ↔ PkgC** recap (Day 17 bug shows up here as cross-IP). The **L0p mid-FLIT** Gen6 bug — what it costs at PM intersection. 10-step debug playbook for "no PkgC10".

---

# Days 23-24 — Wrap

No new exercises. Re-run any 2-3 of D08 / D11 / D17 / D20 / D22 cold, on a clean checkout, as warm-ups before the loop.

- **Day 23** is the MSFT/NPU exit-pointer day; the only coding tie-in is to re-read [d06_page_walk](solutions/d06_page_walk.c) so the QNN page-fault story flows.
- **Day 24** is the final timed mock loop; the exercises are the *content* you point at in the resume walk-through, not new code.

---

## Reading map

Every day file lives in [../prep_plan/](../prep_plan/). Architect drills, bring-up flow, tools (pre-Si + post-Si), and bug anchors are in the day file — coding is in this file. Don't duplicate; cross-link.
