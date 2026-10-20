#include <assert.h>
#include <stdint.h>
#include <stdio.h>

typedef struct { uint32_t freq_mhz, volt_mv; } opp_t;
typedef struct { uint64_t insns, cycles; } pmu_t;
static const char *last_sequence[2];

static const opp_t *select_opp(uint32_t target_mhz, int tj_c) {
    /* TODO: select an OPP and cap frequency at 1200 MHz at/above 90 C. */
    (void)target_mhz;
    (void)tj_c;
    return NULL;
}

static void apply_opp(const opp_t *current, const opp_t *next) {
    /* TODO: ramp voltage before frequency on upshift; reverse on downshift. */
    (void)current;
    (void)next;
}

static double ipc(const pmu_t *pmu) {
    /* TODO: handle a zero cycle count without dividing by zero. */
    (void)pmu;
    return 0.0;
}

int main(void) {
    const opp_t *low = select_opp(400, 50);
    const opp_t *high = select_opp(2000, 50);
    const opp_t *throttled = select_opp(2400, 95);
    assert(low != NULL && high != NULL && throttled != NULL);
    assert(low->freq_mhz == 400 && high->freq_mhz == 2400);
    assert(throttled->freq_mhz == 1200 && select_opp(1201, 90)->freq_mhz == 1200);
    assert(select_opp(0, 50)->freq_mhz == 400);
    assert(select_opp(UINT32_MAX, 50)->freq_mhz == 2400);
    apply_opp(low, high);
    assert(last_sequence[0] != NULL && last_sequence[1] != NULL);
    assert(last_sequence[0][0] == 'v' && last_sequence[1][0] == 'f');
    apply_opp(high, throttled);
    assert(last_sequence[0][0] == 'f' && last_sequence[1][0] == 'v');
    pmu_t pmu = {.insns = 150, .cycles = 100};
    assert(ipc(&pmu) == 1.5);
    pmu.cycles = 0;
    assert(ipc(&pmu) == 0.0 && ipc(NULL) == 0.0);
    puts("s16_dvfs: all assertions passed");
    return 0;
}
