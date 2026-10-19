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
    uint32_t tail = atomic_load_explicit(&ring->tail, memory_order_relaxed);
    uint32_t head = atomic_load_explicit(&ring->head, memory_order_acquire);
    if (tail - head == R) return false;
    ring->buf[tail % R] = value;
    atomic_store_explicit(&ring->tail, tail + 1, memory_order_release);
    return true;
}

static bool spsc_pop(spsc_t *ring, int *value) {
    uint32_t head = atomic_load_explicit(&ring->head, memory_order_relaxed);
    uint32_t tail = atomic_load_explicit(&ring->tail, memory_order_acquire);
    if (head == tail) return false;
    *value = ring->buf[head % R];
    atomic_store_explicit(&ring->head, head + 1, memory_order_release);
    return true;
}

int main(void) {
    spsc_t ring = {0};
    int value;
    assert(!spsc_pop(&ring, &value));
    for (int i = 0; i < R; i++) assert(spsc_push(&ring, i * i));
    assert(!spsc_push(&ring, 99));
    for (int i = 0; i < R; i++) assert(spsc_pop(&ring, &value) && value == i * i);
    assert(!spsc_pop(&ring, &value));
    puts("s09_spsc_acqrel: all assertions passed");
    return 0;
}