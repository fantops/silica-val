#include <assert.h>
#include <stdint.h>
#include <stdio.h>

typedef struct { uint32_t mem[256]; uint32_t wptr, rptr; int depth; } afifo_t;

static uint32_t bin_to_gray(uint32_t value) { return value ^ (value >> 1); }
static int valid_depth(int depth) {
    return depth >= 2 && depth <= 256 && (depth & (depth - 1)) == 0;
}

static int fifo_empty(const afifo_t *fifo) {
    return valid_depth(fifo->depth) && bin_to_gray(fifo->wptr) == bin_to_gray(fifo->rptr);
}

static int fifo_full(const afifo_t *fifo) {
    if (!valid_depth(fifo->depth)) return 0;
    unsigned address_bits = 0;
    for (unsigned depth = (unsigned)fifo->depth; depth > 1; depth >>= 1) address_bits++;
    uint32_t wrap_mask = (1u << address_bits) | (1u << (address_bits - 1));
    return bin_to_gray(fifo->wptr) == (bin_to_gray(fifo->rptr) ^ wrap_mask);
}

static int fifo_write(afifo_t *fifo, uint32_t value) {
    if (fifo == NULL || !valid_depth(fifo->depth) || fifo_full(fifo)) return -1;
    fifo->mem[fifo->wptr % (uint32_t)fifo->depth] = value;
    fifo->wptr = (fifo->wptr + 1) % (2u * (uint32_t)fifo->depth);
    return 0;
}

static int fifo_read(afifo_t *fifo, uint32_t *value) {
    if (fifo == NULL || value == NULL || !valid_depth(fifo->depth) || fifo_empty(fifo)) return -1;
    *value = fifo->mem[fifo->rptr % (uint32_t)fifo->depth];
    fifo->rptr = (fifo->rptr + 1) % (2u * (uint32_t)fifo->depth);
    return 0;
}

static int peak_backlog(int wr_per100, int rd_per10, int clocks, unsigned seed) {
    if (wr_per100 < 0 || wr_per100 > 100 || rd_per10 < 0 || rd_per10 > 10 || clocks <= 0) return -1;
    uint32_t random = seed ? seed : 1;
    int backlog = 0, peak = 0;
    for (int tick = 0; tick < clocks; tick++) {
        random = random * 1664525u + 1013904223u;
        int writes = (int)(random % 100u) < wr_per100 ? 1 : 0;
        random = random * 1664525u + 1013904223u;
        int reads = (int)(random % 10u) < rd_per10 ? 1 : 0;
        backlog += writes - reads;
        if (backlog < 0) backlog = 0;
        if (backlog > peak) peak = backlog;
    }
    return peak;
}

int main(void) {
    afifo_t fifo = {.wptr = 0, .rptr = 0, .depth = 4};
    uint32_t value;
    for (uint32_t i = 0; i < 4; i++) assert(fifo_write(&fifo, i + 10) == 0);
    assert(fifo_full(&fifo) && fifo_write(&fifo, 99) == -1);
    for (uint32_t i = 0; i < 4; i++) {
        assert(fifo_read(&fifo, &value) == 0 && value == i + 10);
    }
    assert(fifo_empty(&fifo) && fifo_read(&fifo, &value) == -1);
    assert(fifo_write(&fifo, 77) == 0 && fifo_read(&fifo, &value) == 0 && value == 77);
    uint32_t gray_change = bin_to_gray(7) ^ bin_to_gray(8);
    assert(gray_change != 0 && (gray_change & (gray_change - 1)) == 0);
    assert(peak_backlog(80, 8, 1000, 1) >= 0);
    puts("s04_async_fifo: all assertions passed");
    return 0;
}