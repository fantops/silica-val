#include <assert.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint32_t mem[256];
    uint32_t wptr, rptr, write_sync1, write_sync2, read_sync1, read_sync2;
    int depth;
} afifo_t;

static uint32_t bin_to_gray(uint32_t value) { return value ^ (value >> 1); }
static int valid_depth(int depth) {
    return depth >= 2 && depth <= 256 && (depth & (depth - 1)) == 0;
}

static int fifo_empty(const afifo_t *fifo) {
    return valid_depth(fifo->depth) && fifo->write_sync2 == bin_to_gray(fifo->rptr);
}

static void write_clock(afifo_t *fifo) {
    fifo->read_sync2 = fifo->read_sync1;
    fifo->read_sync1 = bin_to_gray(fifo->rptr);
}

static void read_clock(afifo_t *fifo) {
    fifo->write_sync2 = fifo->write_sync1;
    fifo->write_sync1 = bin_to_gray(fifo->wptr);
}

static int fifo_full(const afifo_t *fifo) {
    if (!valid_depth(fifo->depth)) return 0;
    unsigned address_bits = 0;
    for (unsigned depth = (unsigned)fifo->depth; depth > 1; depth >>= 1) address_bits++;
    uint32_t wrap_mask = (1u << address_bits) | (1u << (address_bits - 1));
    return bin_to_gray(fifo->wptr) == (fifo->read_sync2 ^ wrap_mask);
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

static int peak_backlog(const int *writes, const int *reads, int clocks) {
    if (writes == NULL || reads == NULL || clocks < 0) return -1;
    int backlog = 0, peak = 0;
    for (int tick = 0; tick < clocks; tick++) {
        if ((writes[tick] != 0 && writes[tick] != 1) ||
            (reads[tick] != 0 && reads[tick] != 1)) return -1;
        backlog += writes[tick] - reads[tick];
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
    assert(fifo_empty(&fifo));
    read_clock(&fifo);
    assert(fifo_empty(&fifo));
    read_clock(&fifo);
    assert(!fifo_empty(&fifo));
    for (uint32_t i = 0; i < 4; i++) {
        assert(fifo_read(&fifo, &value) == 0 && value == i + 10);
    }
    assert(fifo_empty(&fifo) && fifo_read(&fifo, &value) == -1);
    assert(fifo_full(&fifo));
    write_clock(&fifo);
    assert(fifo_full(&fifo));
    write_clock(&fifo);
    assert(!fifo_full(&fifo));
    for (uint32_t lap = 0; lap < 16; lap++) {
        assert(fifo_write(&fifo, lap + 77) == 0);
        read_clock(&fifo);
        read_clock(&fifo);
        assert(fifo_read(&fifo, &value) == 0 && value == lap + 77);
        write_clock(&fifo);
        write_clock(&fifo);
    }
    uint32_t gray_change = bin_to_gray(7) ^ bin_to_gray(8);
    assert(gray_change != 0 && (gray_change & (gray_change - 1)) == 0);
    const int writes[] = {1,1,1,1,1,1,1,1,0,0,0,0};
    const int reads[] = {0,0,0,0,1,1,1,1,1,1,1,1};
    assert(peak_backlog(writes, reads, 12) == 4);
    assert(peak_backlog(writes, reads, -1) == -1);
    puts("s04_async_fifo: all assertions passed");
    return 0;
}