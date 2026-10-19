#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef enum { LTSSM_DETECT, LTSSM_POLLING, LTSSM_CONFIG, LTSSM_L0,
               LTSSM_RECOVERY, LTSSM_L0S, LTSSM_L1, LTSSM_L1_1,
               LTSSM_L1_2, LTSSM_L2, LTSSM_NUM } ltssm_t;
typedef struct { ltssm_t st; unsigned recoveries; } link_t;

static bool allowed(ltssm_t from, ltssm_t to) {
    switch (from) {
    case LTSSM_DETECT: return to == LTSSM_POLLING;
    case LTSSM_POLLING: return to == LTSSM_CONFIG || to == LTSSM_DETECT;
    case LTSSM_CONFIG: return to == LTSSM_L0 || to == LTSSM_DETECT;
    case LTSSM_L0: return to == LTSSM_RECOVERY || to == LTSSM_L0S || to == LTSSM_L1 || to == LTSSM_L2;
    case LTSSM_RECOVERY: return to == LTSSM_L0 || to == LTSSM_POLLING;
    case LTSSM_L0S: return to == LTSSM_L0 || to == LTSSM_L1;
    case LTSSM_L1: return to == LTSSM_L0 || to == LTSSM_L1_1;
    case LTSSM_L1_1: return to == LTSSM_L1 || to == LTSSM_L1_2;
    case LTSSM_L1_2: return to == LTSSM_L1;
    case LTSSM_L2: return to == LTSSM_DETECT;
    default: return false;
    }
}

static bool go(link_t *link, ltssm_t to) {
    if (link == NULL || to < 0 || to >= LTSSM_NUM || !allowed(link->st, to)) return false;
    link->st = to;
    if (to == LTSSM_RECOVERY) link->recoveries++;
    return true;
}

static uint64_t bar_size_from_readback(uint32_t readback) {
    uint32_t address_mask = readback & ~0x0fu;
    return (uint32_t)(~address_mask) + 1u;
}

int main(void) {
    link_t link = {.st = LTSSM_DETECT};
    const ltssm_t path[] = {LTSSM_POLLING, LTSSM_CONFIG, LTSSM_L0, LTSSM_RECOVERY,
                            LTSSM_L0, LTSSM_L1, LTSSM_L1_1, LTSSM_L1_2, LTSSM_L1,
                            LTSSM_L0, LTSSM_RECOVERY, LTSSM_L0};
    for (unsigned i = 0; i < sizeof(path) / sizeof(path[0]); i++) assert(go(&link, path[i]));
    assert(link.recoveries == 2);
    link.st = LTSSM_DETECT;
    assert(!go(&link, LTSSM_L0));
    assert(bar_size_from_readback(0xFFF00004u) == 0x100000u);
    puts("s10_ltssm_walk: all assertions passed");
    return 0;
}