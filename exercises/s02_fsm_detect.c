#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static int seq_detect(const int *bits, int n, uint32_t pattern, int plen, int overlapping, int *out) {
    /* TODO: detect the bit pattern and optionally resume from overlapping suffixes. */
    (void)bits;
    (void)n;
    (void)pattern;
    (void)plen;
    (void)overlapping;
    (void)out;
    return -1;
}

static int win_match(const int *bits, int n, uint32_t pattern, int plen, int *out) {
    /* TODO: implement a sliding-window oracle for seq_detect. */
    (void)bits;
    (void)n;
    (void)pattern;
    (void)plen;
    (void)out;
    return -1;
}

static int safe_fsm_step(int state, int in, int inject_illegal) {
    /* TODO: transition safely; illegal states must recover to state zero. */
    (void)state;
    (void)in;
    (void)inject_illegal;
    return -1;
}

int main(void) {
    const int bits[] = {1, 0, 1, 0, 1};
    int out[5];
    assert(seq_detect(bits, 5, 5, 3, 1, out) == 2);
    assert(seq_detect(bits, 5, 5, 3, 0, out) == 1);
    assert(win_match(bits, 5, 5, 3, out) == 2);
    assert(safe_fsm_step(99, 1, 0) == 0);
    assert(safe_fsm_step(2, 1, 1) == 0);
    puts("Complete the TODOs to pass s02_fsm_detect.");
    return 0;
}