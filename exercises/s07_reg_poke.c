#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#define BIT(n) (UINT32_C(1) << (n))

static uint32_t field_mask(unsigned msb, unsigned lsb) {
    /* TODO: return a mask covering [msb:lsb], guarding invalid ranges. */
    (void)msb;
    (void)lsb;
    return 0;
}

static uint32_t reg_read32(volatile uint32_t *reg) { return *reg; }
static void reg_write32(volatile uint32_t *reg, uint32_t value) { *reg = value; }

static void reg_field_write(volatile uint32_t *reg, unsigned msb, unsigned lsb, uint32_t value) {
    /* TODO: perform an RMW that leaves every bit outside the field unchanged. */
    (void)reg;
    (void)msb;
    (void)lsb;
    (void)value;
}

static int wait_for_bit_set(volatile uint32_t *reg, uint32_t mask, unsigned tries) {
    /* TODO: poll no more than tries times; return 0 on success and -1 on timeout. */
    (void)reg;
    (void)mask;
    (void)tries;
    return -1;
}

int main(void) {
    volatile uint32_t reg = 0xDEADBEEFu;
    reg_field_write(&reg, 15, 8, 0xAA);
    assert(reg == 0xDEADAAEFu);
    reg_field_write(&reg, 32, 32, UINT32_MAX);
    reg_field_write(&reg, 3, 4, UINT32_MAX);
    assert(reg == 0xDEADAAEFu);
    reg_field_write(&reg, 31, 0, UINT32_MAX);
    assert(reg == UINT32_MAX);
    reg_field_write(&reg, 31, 31, 0);
    assert(reg == UINT32_C(0x7fffffff));
    reg = 0;
    assert(wait_for_bit_set(&reg, BIT(0), 5) == -1);
    reg = BIT(0);
    assert(wait_for_bit_set(&reg, BIT(0), 5) == 0);
    assert(wait_for_bit_set(&reg, BIT(0), 0) == -1);
    puts("s07_reg_poke: all assertions passed");
    return 0;
}
