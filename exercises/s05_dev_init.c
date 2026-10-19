#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define CTRL 0x00u
#define STATUS 0x04u
#define DATA 0x08u
#define CTRL_EN (1u << 0)
#define CTRL_START (1u << 1)
#define CTRL_RESET (1u << 31)
#define STATUS_RDY (1u << 0)

static volatile uint32_t mmio[64];

static uint32_t reg_read(uint32_t off) { return mmio[off / 4]; }
static void reg_write(uint32_t off, uint32_t value) { mmio[off / 4] = value; }
static void reg_rmw(uint32_t off, uint32_t mask, uint32_t value) {
    /* TODO: preserve unrelated control bits. */
    (void)off;
    (void)mask;
    (void)value;
}

static bool poll_ready(uint32_t off, uint32_t mask, uint32_t expected, uint32_t tries) {
    /* TODO: bounded poll; simulate ready becoming set on the third read. */
    (void)off;
    (void)mask;
    (void)expected;
    (void)tries;
    return false;
}

int main(void) {
    reg_write(CTRL, CTRL_RESET);
    reg_write(CTRL, 0);
    assert(poll_ready(STATUS, STATUS_RDY, STATUS_RDY, 5));
    reg_rmw(CTRL, CTRL_EN, CTRL_EN);
    reg_rmw(CTRL, CTRL_START, CTRL_START);
    assert((reg_read(CTRL) & (CTRL_EN | CTRL_START)) == (CTRL_EN | CTRL_START));
    reg_write(DATA, 0xDEADBEEFu);
    assert(reg_read(DATA) == 0xDEADBEEFu);
    puts("Complete the TODOs to pass s05_dev_init.");
    return 0;
}