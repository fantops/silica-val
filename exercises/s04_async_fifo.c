#include <assert.h>
#include <stdint.h>
#include <stdio.h>

typedef struct { uint32_t mem[256]; uint32_t wptr, rptr; int depth; } afifo_t;

static uint32_t bin_to_gray(uint32_t value) {
    /* TODO: Gray encoding changes exactly one bit between adjacent values. */
    (void)value;
    return 0;
}

static int fifo_empty(const afifo_t *fifo) {
    return bin_to_gray(fifo->wptr) == bin_to_gray(fifo->rptr);
}
static int fifo_full(const afifo_t *fifo) {
    /* TODO: account for the extra pointer wrap bit or equivalent occupancy. */
    (void)fifo;
    return 0;
}

static int fifo_write(afifo_t *fifo, uint32_t value) {
    /* TODO: reject full writes; preserve order across ring wrap. */
    (void)fifo;
    (void)value;
    return -1;
}

static int fifo_read(afifo_t *fifo, uint32_t *value) {
    /* TODO: reject empty reads without returning stale data. */
    (void)fifo;
    (void)value;
    return -1;
}

static int peak_backlog(int wr_per100, int rd_per10, int clocks, unsigned seed) {
    /* TODO: model burst arrivals and return the maximum queued item count. */
    (void)wr_per100;
    (void)rd_per10;
    (void)clocks;
    (void)seed;
    return -1;
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
    puts("Complete the TODOs to pass s04_async_fifo.");
    return 0;
}