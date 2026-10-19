#include <assert.h>
#include <stdint.h>
#include <stdio.h>

typedef struct { uint32_t freq_mhz, volt_mv; } opp_t;
typedef struct { uint64_t insns, cycles; } pmu_t;
static const opp_t table[] = {{400, 750}, {800, 850}, {1200, 950}, {1800, 1050}, {2400, 1150}};
static const char *last_sequence[2];

static const opp_t *select_opp(uint32_t target_mhz, int tj_c) {
    uint32_t cap = tj_c >= 90 ? 1200 : 2400;
    const opp_t *best = &table[0];
    for (unsigned i = 0; i < sizeof(table) / sizeof(table[0]); i++) {
        if (table[i].freq_mhz <= cap && table[i].freq_mhz >= target_mhz) return &table[i];
        if (table[i].freq_mhz <= cap) best = &table[i];
    }
    return best;
}

static void apply_opp(const opp_t *current, const opp_t *next) {
    if (next->freq_mhz > current->freq_mhz) {
        last_sequence[0] = "voltage";
        last_sequence[1] = "frequency";
    } else {
        last_sequence[0] = "frequency";
        last_sequence[1] = "voltage";
    }
    printf("DVFS order: %s then %s\n", last_sequence[0], last_sequence[1]);
}

static double ipc(const pmu_t *pmu) {
    return pmu == NULL || pmu->cycles == 0 ? 0.0 : (double)pmu->insns / (double)pmu->cycles;
}

int main(void) {
    const opp_t *low = select_opp(400, 50);
    const opp_t *high = select_opp(2000, 50);
    const opp_t *throttled = select_opp(2400, 95);
    assert(low->freq_mhz == 400 && high->freq_mhz == 2400);
    assert(throttled->freq_mhz == 1200);
    apply_opp(low, high);
    assert(last_sequence[0][0] == 'v');
    apply_opp(high, throttled);
    assert(last_sequence[0][0] == 'f');
    pmu_t pmu = {.insns = 150, .cycles = 100};
    assert(ipc(&pmu) == 1.5);
    puts("s16_dvfs: all assertions passed");
    return 0;
}