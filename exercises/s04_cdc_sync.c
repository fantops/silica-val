#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static int toggle_sync(const int *src_pulses, int n, int dest_div, int *dest_pulses) {
    /* TODO: toggle on source events, sample every dest_div cycles, edge-detect. */
    (void)src_pulses;
    (void)n;
    (void)dest_div;
    (void)dest_pulses;
    return -1;
}

static unsigned handshake_ticks;

static int handshake_xfer(const uint32_t *words, int n, uint32_t *out) {
    /* TODO: transfer all words in order using a closed-loop handshake model. */
    (void)words;
    (void)n;
    (void)out;
    return -1;
}

static uint32_t bin_to_gray(uint32_t value) {
    /* TODO: convert a binary pointer to Gray code. */
    (void)value;
    return 0;
}

int main(void) {
    const int spaced[] = {0, 1, 0, 0, 1, 0, 0, 1};
    int out[8];
    assert(toggle_sync(spaced, 8, 2, out) == 3);
    const int back_to_back[] = {1, 1, 0, 0};
    assert(toggle_sync(back_to_back, 4, 2, out) == 0);
    const uint32_t words[] = {0x12, 0x34, 0x56};
    uint32_t received[3];
    assert(handshake_xfer(words, 3, received) == 3);
    assert(handshake_ticks > 3);
    for (int i = 0; i < 3; i++) assert(received[i] == words[i]);
    assert(handshake_xfer(words, 0, received) == 0 && handshake_ticks == 0);
    assert(bin_to_gray(7) == 4);
    puts("s04_cdc_sync: all assertions passed");
    return 0;
}
