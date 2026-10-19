#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#define BIT(n) (UINT32_C(1) << (n))

static uint32_t field_mask(unsigned msb, unsigned lsb) {
    if (msb >= 32 || lsb > msb) return 0;
    unsigned width = msb - lsb + 1;
    return width == 32 ? UINT32_MAX : (BIT(width) - 1u) << lsb;
}

static uint32_t reg_read32(volatile uint32_t *reg) { return *reg; }
static void reg_write32(volatile uint32_t *reg, uint32_t value) { *reg = value; }

static void reg_field_write(volatile uint32_t *reg, unsigned msb, unsigned lsb, uint32_t value) {
    uint32_t mask = field_mask(msb, lsb);
    unsigned shift = lsb;
    reg_write32(reg, (reg_read32(reg) & ~mask) | ((value << shift) & mask));
}

static int wait_for_bit_set(volatile uint32_t *reg, uint32_t mask, unsigned tries) {
    for (unsigned i = 0; i < tries; i++) {
        if ((reg_read32(reg) & mask) == mask) return 0;
    }
    return -1;
}

int main(void) {
    volatile uint32_t reg = 0xDEADBEEFu;
    reg_field_write(&reg, 15, 8, 0xAA);
    assert(reg == 0xDEADAAEFu);
    reg = 0;
    assert(wait_for_bit_set(&reg, BIT(0), 5) == -1);
    reg = BIT(0);
    assert(wait_for_bit_set(&reg, BIT(0), 5) == 0);
    puts("s07_reg_poke: all assertions passed");
    return 0;
}