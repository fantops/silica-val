#include <assert.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint64_t cc3, cc6, cc7;
    uint64_t pc2, pc6, pc10;
    uint64_t tsc;
} msr_snap_t;

static uint64_t wrap_delta(uint64_t end, uint64_t start) { return end - start; }

static double residency_pct(uint64_t end, uint64_t start, uint64_t wall_delta, int *sanity) {
    uint64_t delta = wrap_delta(end, start);
    if (sanity != NULL) *sanity = delta > wall_delta;
    return wall_delta == 0 ? 0.0 : 100.0 * (double)delta / (double)wall_delta;
}

static void print_row(const char *label, const msr_snap_t *start, const msr_snap_t *end) {
    uint64_t wall = wrap_delta(end->tsc, start->tsc);
    const char *names[] = {"CC3", "CC6", "CC7", "PC2", "PC6", "PC10"};
    const uint64_t before[] = {start->cc3, start->cc6, start->cc7, start->pc2, start->pc6, start->pc10};
    const uint64_t after[] = {end->cc3, end->cc6, end->cc7, end->pc2, end->pc6, end->pc10};
    printf("%s TSC=%llu", label, (unsigned long long)wall);
    for (unsigned i = 0; i < sizeof(names) / sizeof(names[0]); i++) {
        int sanity = 0;
        double pct = residency_pct(after[i], before[i], wall, &sanity);
        printf(" %s=%.2f%%%s", names[i], pct, sanity ? "!" : "");
    }
    putchar('\n');
}

int main(void) {
    const uint64_t start = UINT64_C(0xffffffffffffff00);
    assert(wrap_delta(0x100, start) == 0x200);
    int sanity = 0;
    assert(residency_pct(0x100, start, 0x400, &sanity) == 50.0 && !sanity);
    assert(residency_pct(200, 0, 100, &sanity) == 200.0 && sanity);
    msr_snap_t t0 = {.cc6 = start, .tsc = 0};
    msr_snap_t t1 = {.cc6 = 0x100, .pc6 = 0x500, .tsc = 0x400};
    print_row("sample", &t0, &t1);
    puts("s16_residency_delta: all assertions passed");
    return 0;
}