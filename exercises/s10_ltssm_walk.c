#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef enum { LTSSM_DETECT, LTSSM_POLLING, LTSSM_CONFIG, LTSSM_L0,
               LTSSM_RECOVERY, LTSSM_L0S, LTSSM_L1, LTSSM_L1_1,
               LTSSM_L1_2, LTSSM_L2, LTSSM_NUM } ltssm_t;
typedef struct { ltssm_t st; unsigned recoveries; } link_t;

static bool go(link_t *link, ltssm_t to) {
    /* TODO: enforce the legal LTSSM transition table and count Recovery entries. */
    (void)link;
    (void)to;
    return false;
}

static uint64_t bar_size_from_readback(uint32_t readback) {
    /* TODO: ignore BAR type bits, invert the decoder mask, and add one. */
    (void)readback;
    return 0;
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
