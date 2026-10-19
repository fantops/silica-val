#include <assert.h>
#include <stdint.h>
#include <stdio.h>

typedef struct { uint32_t freq_mhz, volt_mv; } opp_t;
typedef struct { uint64_t insns, cycles; } pmu_t;

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
    assert(low != NULL && low->freq_mhz == 400 && high != NULL && high->freq_mhz == 2400);
    assert(throttled != NULL && throttled->freq_mhz == 1200);
    apply_opp(low, high);
    apply_opp(high, throttled);
    pmu_t pmu = {.insns = 150, .cycles = 100};
    assert(ipc(&pmu) == 1.5);
    puts("Complete the TODOs to pass s16_dvfs.");
    return 0;
}