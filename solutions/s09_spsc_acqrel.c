#include <assert.h>
#include <stdbool.h>
#include <stdalign.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#include <sched.h>
#endif

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

#define TRANSFERS 100000

static void yield_thread(void) {
#if defined(_WIN32)
    SwitchToThread();
#else
    sched_yield();
#endif
}

static void produce(spsc_t *ring) {
    for (int value = 0; value < TRANSFERS; value++) {
        while (!spsc_push(ring, value)) yield_thread();
    }
}

static void consume(spsc_t *ring) {
    for (int expected = 0; expected < TRANSFERS; expected++) {
        int value;
        while (!spsc_pop(ring, &value)) yield_thread();
        assert(value == expected);
    }
}

#if defined(_WIN32)
static DWORD WINAPI producer_thread(LPVOID ring) { produce(ring); return 0; }
static DWORD WINAPI consumer_thread(LPVOID ring) { consume(ring); return 0; }
#else
static void *producer_thread(void *ring) { produce(ring); return NULL; }
static void *consumer_thread(void *ring) { consume(ring); return NULL; }
#endif

static void concurrent_test(spsc_t *ring) {
#if defined(_WIN32)
    HANDLE producer = CreateThread(NULL, 0, producer_thread, ring, 0, NULL);
    HANDLE consumer = CreateThread(NULL, 0, consumer_thread, ring, 0, NULL);
    assert(producer != NULL && consumer != NULL);
    assert(WaitForSingleObject(producer, INFINITE) == WAIT_OBJECT_0);
    assert(WaitForSingleObject(consumer, INFINITE) == WAIT_OBJECT_0);
    assert(CloseHandle(producer) && CloseHandle(consumer));
#else
    pthread_t producer, consumer;
    assert(pthread_create(&producer, NULL, producer_thread, ring) == 0);
    assert(pthread_create(&consumer, NULL, consumer_thread, ring) == 0);
    assert(pthread_join(producer, NULL) == 0);
    assert(pthread_join(consumer, NULL) == 0);
#endif
}

int main(void) {
    spsc_t ring = {0};
    int value;
    assert(!spsc_pop(&ring, &value));
    for (int i = 0; i < R; i++) assert(spsc_push(&ring, i * i));
    assert(!spsc_push(&ring, 99));
    for (int i = 0; i < R; i++) assert(spsc_pop(&ring, &value) && value == i * i);
    assert(!spsc_pop(&ring, &value));
    atomic_store(&ring.head, UINT32_MAX - 4);
    atomic_store(&ring.tail, UINT32_MAX - 4);
    concurrent_test(&ring);
    assert(atomic_load(&ring.head) == atomic_load(&ring.tail));
    assert(!spsc_pop(&ring, &value));
    puts("s09_spsc_acqrel: all assertions passed");
    return 0;
}