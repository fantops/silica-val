#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static int seq_detect(const int *bits, int n, uint32_t pattern, int plen, int overlapping, int *out) {
    if (bits == NULL || out == NULL || plen <= 0 || plen > 32 || n < 0) return -1;
    int found = 0;
    for (int i = 0; i < n; i++) out[i] = 0;
    for (int end = plen - 1; end < n; end++) {
        int match = 1;
        for (int bit = 0; bit < plen; bit++) {
            if (bits[end - plen + 1 + bit] != (int)((pattern >> (plen - 1 - bit)) & 1u)) {
                match = 0;
                break;
            }
        }
        if (match) {
            out[end] = 1;
            found++;
            if (!overlapping) end += plen - 1;
        }
    }
    return found;
}

static int win_match(const int *bits, int n, uint32_t pattern, int plen, int *out) {
    return seq_detect(bits, n, pattern, plen, 1, out);
}

static int safe_fsm_step(int state, int in, int inject_illegal) {
    if (inject_illegal || state < 0 || state > 2) return 0;
    if (state == 0) return in ? 1 : 0;
    if (state == 1) return in ? 1 : 2;
    return in ? 1 : 0;
}

int main(void) {
    const int bits[] = {1, 0, 1, 0, 1};
    int out[5];
    assert(seq_detect(bits, 5, 5, 3, 1, out) == 2);
    assert(seq_detect(bits, 5, 5, 3, 0, out) == 1);
    assert(win_match(bits, 5, 5, 3, out) == 2);
    assert(safe_fsm_step(99, 1, 0) == 0);
    assert(safe_fsm_step(2, 1, 1) == 0);
    puts("s02_fsm_detect: all assertions passed");
    return 0;
}