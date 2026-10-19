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
    reg_write(off, (reg_read(off) & ~mask) | (value & mask));
}

static bool poll_ready(uint32_t off, uint32_t mask, uint32_t expected, uint32_t tries) {
    for (uint32_t i = 0; i < tries; i++) {
        if (i == 2) mmio[off / 4] |= mask;
        if ((reg_read(off) & mask) == expected) return true;
    }
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
    puts("s05_dev_init: all assertions passed");
    return 0;
}