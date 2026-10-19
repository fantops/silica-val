#include <assert.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint64_t cc3, cc6, cc7;
    uint64_t pc2, pc6, pc10;
    uint64_t tsc;
} msr_snap_t;

static uint64_t wrap_delta(uint64_t end, uint64_t start) {
    /* TODO: compute an unsigned modulo-2^64 delta. */
    (void)end;
    (void)start;
    return 0;
}

static double residency_pct(uint64_t end, uint64_t start, uint64_t wall_delta, int *sanity) {
    /* TODO: report percentage and flag residency greater than elapsed wall ticks. */
    (void)end;
    (void)start;
    (void)wall_delta;
    (void)sanity;
    return 0.0;
}

static void print_row(const char *label, const msr_snap_t *start, const msr_snap_t *end) {
    /* TODO: print all core/package counters with wrap-safe percentages and sanity flags. */
    (void)label;
    (void)start;
    (void)end;
}

int main(void) {
    const uint64_t start = UINT64_C(0xffffffffffffff00);
    assert(wrap_delta(0x100, start) == 0x200);
    int sanity = 0;
    assert(residency_pct(0x100, start, 0x400, &sanity) == 50.0 && !sanity);
    assert(residency_pct(200, 0, 100, &sanity) == 200.0 && sanity);
    msr_snap_t t0 = {.cc6 = start, .tsc = 0};
    msr_snap_t t1 = {.cc6 = 0x100, .tsc = 0x400};
    print_row("sample", &t0, &t1);
    puts("Complete the TODOs to pass s16_residency_delta.");
    return 0;
}