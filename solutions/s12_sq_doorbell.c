#include <assert.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>

#define SQ_SIZE 16
typedef struct {
    uint32_t cmd, arg;
    uint64_t completion_addr, src_addr, dst_addr;
    uint32_t length, flags;
    uint32_t reserved[6];
} sqe_t;
_Static_assert(sizeof(sqe_t) == 64, "submission descriptors are 64 bytes");
static sqe_t queue[SQ_SIZE];
static unsigned head, tail;
static volatile uint32_t doorbell;

static int sq_submit(uint32_t cmd, uint32_t arg) {
    if (tail - head >= SQ_SIZE) return -1;
    queue[tail % SQ_SIZE] = (sqe_t){.cmd = cmd, .arg = arg};
    atomic_thread_fence(memory_order_release);
    tail++;
    doorbell = tail;
    return 0;
}

static int device_consume_one(sqe_t *out) {
    if (head == tail) return -1;
    atomic_thread_fence(memory_order_acquire);
    *out = queue[head % SQ_SIZE];
    head++;
    return 0;
}

int main(void) {
    sqe_t consumed;
    assert(sq_submit(1, 0xAAAA) == 0 && doorbell == 1);
    assert(sq_submit(2, 0xBBBB) == 0 && doorbell == 2);
    assert(device_consume_one(&consumed) == 0 && consumed.cmd == 1 && consumed.arg == 0xAAAA);
    assert(device_consume_one(&consumed) == 0 && consumed.cmd == 2 && consumed.arg == 0xBBBB);
    assert(device_consume_one(&consumed) == -1);
    for (uint32_t i = 0; i < SQ_SIZE; i++) assert(sq_submit(i, i + 1) == 0);
    assert(sq_submit(99, 99) == -1);
    for (uint32_t i = 0; i < SQ_SIZE; i++) {
        assert(device_consume_one(&consumed) == 0 && consumed.cmd == i && consumed.arg == i + 1);
    }
    puts("s12_sq_doorbell: all assertions passed");
    return 0;
}