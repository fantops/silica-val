#include <assert.h>
#include <stdbool.h>
#include <stdalign.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>

#define R 8
typedef struct {
    int buf[R];
    alignas(64) _Atomic uint32_t head;
    alignas(64) _Atomic uint32_t tail;
} spsc_t;

static bool spsc_push(spsc_t *ring, int value) {
    /* TODO: acquire head before reuse; publish payload before tail (release). */
    (void)ring;
    (void)value;
    return false;
}

static bool spsc_pop(spsc_t *ring, int *value) {
    /* TODO: acquire tail before reading payload; release the consumed slot. */
    (void)ring;
    (void)value;
    return false;
}

int main(void) {
    spsc_t ring = {0};
    int value;
    assert(!spsc_pop(&ring, &value));
    for (int i = 0; i < R; i++) assert(spsc_push(&ring, i * i));
    assert(!spsc_push(&ring, 99));
    for (int i = 0; i < R; i++) assert(spsc_pop(&ring, &value) && value == i * i);
    assert(!spsc_pop(&ring, &value));
    puts("Complete the TODOs to pass s09_spsc_acqrel.");
    return 0;
}