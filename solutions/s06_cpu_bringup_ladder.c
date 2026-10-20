#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum {
    STG_CLOCKS_STABLE, STG_RESET_DEASSERTED, STG_RAMS_BIST,
    STG_MMU_ENABLED, STG_CACHE_ENABLED, STG_INT_CONTROLLER_UP,
    STG_TIMERS_ONLINE, STG_SMP_RELEASED, STG_BOOT_REACHED, STG_NUM
} stage_t;

typedef struct { bool ready[STG_NUM]; int retries[STG_NUM]; int current; } bringup_t;

static bool advance(bringup_t *state, stage_t next) {
    if (state == NULL || state->current < -1 || state->current >= STG_NUM ||
        next < 0 || next >= STG_NUM || next != (stage_t)(state->current + 1)) return false;
    if (next > 0 && !state->ready[next - 1]) return false;
    state->ready[next] = true;
    state->current = next;
    return true;
}

static bool inject_fail(bringup_t *state, stage_t stage, int max_retries) {
    if (state == NULL || stage < 0 || stage >= STG_NUM || max_retries < 0) return false;
    state->retries[stage]++;
    return state->retries[stage] <= max_retries;
}

int main(void) {
    bringup_t state = {.current = -1};
    assert(!advance(&state, STG_RESET_DEASSERTED));
    assert(inject_fail(&state, STG_RAMS_BIST, 1));
    assert(!inject_fail(&state, STG_RAMS_BIST, 1));
    for (int stage = 0; stage < STG_NUM; stage++) assert(advance(&state, (stage_t)stage));
    assert(state.ready[STG_BOOT_REACHED]);
    puts("s06_cpu_bringup_ladder: all assertions passed");
    return 0;
}