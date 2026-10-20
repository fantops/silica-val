#include <assert.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint32_t mem[256];
    uint32_t wptr, rptr, write_sync1, write_sync2, read_sync1, read_sync2;
    int depth;
} afifo_t;

static uint32_t bin_to_gray(uint32_t value) {
    /* TODO: Gray encoding changes exactly one bit between adjacent values. */
    (void)value;
    return 0;
}

static int fifo_empty(const afifo_t *fifo) {
    return fifo->write_sync2 == bin_to_gray(fifo->rptr);
}

static void write_clock(afifo_t *fifo) {
    /* TODO: advance the two read-pointer synchronizer stages in the write domain. */
    (void)fifo;
}

static void read_clock(afifo_t *fifo) {
    /* TODO: advance the two write-pointer synchronizer stages in the read domain. */
    (void)fifo;
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

static int peak_backlog(const int *writes, const int *reads, int clocks) {
    /* TODO: calculate peak end-of-tick backlog for explicit 0/1 schedules. */
    (void)writes;
    (void)reads;
    (void)clocks;
    return -1;
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
