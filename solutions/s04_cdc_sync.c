#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static int toggle_sync(const int *src_pulses, int n, int dest_div, int *dest_pulses) {
    if (src_pulses == NULL || dest_pulses == NULL || n < 0 || dest_div <= 0) return -1;
    int source_toggle = 0, sampled_toggle = 0, delivered = 0;
    for (int i = 0; i < n; i++) {
        dest_pulses[i] = 0;
        if (src_pulses[i]) source_toggle = !source_toggle;
        if ((i + 1) % dest_div == 0 || i == n - 1) {
            if (source_toggle != sampled_toggle) {
                dest_pulses[i] = 1;
                sampled_toggle = source_toggle;
                delivered++;
            }
        }
    }
    return delivered;
}

static int handshake_xfer(const uint32_t *words, int n, uint32_t *out) {
    if (words == NULL || out == NULL || n < 0) return -1;
    for (int i = 0; i < n; i++) out[i] = words[i];
    return n;
}

static uint32_t bin_to_gray(uint32_t value) { return value ^ (value >> 1); }

int main(void) {
    const int spaced[] = {0, 1, 0, 0, 1, 0, 0, 1};
    int out[8];
    assert(toggle_sync(spaced, 8, 2, out) == 3);
    const int back_to_back[] = {1, 1, 0, 0};
    assert(toggle_sync(back_to_back, 4, 2, out) == 0);
    const uint32_t words[] = {0x12, 0x34, 0x56};
    uint32_t received[3];
    assert(handshake_xfer(words, 3, received) == 3);
    for (int i = 0; i < 3; i++) assert(received[i] == words[i]);
    assert(bin_to_gray(7) == 4);
    puts("s04_cdc_sync: all assertions passed");
    return 0;
}