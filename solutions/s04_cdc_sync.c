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

static unsigned handshake_ticks;

static int handshake_xfer(const uint32_t *words, int n, uint32_t *out) {
    if (words == NULL || out == NULL || n < 0) return -1;
    int request = 0, acknowledge = 0;
    int request_sync1 = 0, request_sync2 = 0;
    int acknowledge_sync1 = 0, acknowledge_sync2 = 0;
    int submitted = 0, received = 0;
    uint32_t held_data = 0;
    handshake_ticks = 0;
    while (received < n || acknowledge_sync2 != request) {
        acknowledge_sync2 = acknowledge_sync1;
        acknowledge_sync1 = acknowledge;
        if (request == acknowledge_sync2 && submitted < n) {
            held_data = words[submitted++];
            request = !request;
        }
        handshake_ticks++;
        if (handshake_ticks % 3 == 0) {
            request_sync2 = request_sync1;
            request_sync1 = request;
            if (request_sync2 != acknowledge) {
                out[received++] = held_data;
                acknowledge = request_sync2;
            }
        }
    }
    return received;
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
    assert(handshake_ticks > 3);
    for (int i = 0; i < 3; i++) assert(received[i] == words[i]);
    assert(handshake_xfer(words, 0, received) == 0 && handshake_ticks == 0);
    assert(bin_to_gray(7) == 4);
    puts("s04_cdc_sync: all assertions passed");
    return 0;
}