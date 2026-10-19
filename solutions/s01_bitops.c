#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static int is_pow2(uint32_t x) {
    return x != 0 && (x & (x - 1)) == 0;
}

static uint32_t floor_pow2(uint32_t x) {
    if (x == 0) return 0;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x - (x >> 1);
}

static uint32_t ceil_pow2(uint32_t x) {
    if (x <= 1) return x;
    if (x > 0x80000000u) return 0;
    return floor_pow2(x - 1) << 1;
}

static int popcount_kern(uint32_t x) {
    int count = 0;
    while (x != 0) {
        x &= x - 1;
        count++;
    }
    return count;
}

static int popcount_swar(uint32_t x) {
    x = x - ((x >> 1) & 0x55555555u);
    x = (x & 0x33333333u) + ((x >> 2) & 0x33333333u);
    x = (x + (x >> 4)) & 0x0F0F0F0Fu;
    return (int)((x * 0x01010101u) >> 24);
}

static uint32_t get_field(uint32_t value, unsigned low, unsigned width) {
    if (width == 0 || low >= 32 || width > 32 - low) return 0;
    uint32_t mask = width == 32 ? UINT32_MAX : (1u << width) - 1u;
    return (value >> low) & mask;
}

static uint32_t put_field(uint32_t reg, unsigned low, unsigned width, uint32_t value) {
    if (width == 0 || low >= 32 || width > 32 - low) return reg;
    uint32_t field = width == 32 ? UINT32_MAX : (1u << width) - 1u;
    uint32_t mask = field << low;
    return (reg & ~mask) | ((value & field) << low);
}

static uint32_t rol32(uint32_t x, unsigned n) {
    n %= 32;
    return n == 0 ? x : (x << n) | (x >> (32 - n));
}

static uint32_t ror32(uint32_t x, unsigned n) {
    n %= 32;
    return n == 0 ? x : (x >> n) | (x << (32 - n));
}

static int ffs32(uint32_t x) {
    if (x == 0) return 0;
    int bit = 1;
    while ((x & 1u) == 0) { x >>= 1; bit++; }
    return bit;
}

static int clz32(uint32_t x) {
    if (x == 0) return 32;
    int count = 0;
    for (uint32_t mask = 0x80000000u; (x & mask) == 0; mask >>= 1) count++;
    return count;
}

static int ctz32(uint32_t x) {
    if (x == 0) return 32;
    int count = 0;
    while ((x & 1u) == 0) { x >>= 1; count++; }
    return count;
}

static uint32_t bswap32(uint32_t x) {
    return ((x & 0x000000ffu) << 24) | ((x & 0x0000ff00u) << 8) |
           ((x & 0x00ff0000u) >> 8) | ((x & 0xff000000u) >> 24);
}

static uint32_t reverse32(uint32_t x) {
    uint32_t reversed = 0;
    for (unsigned i = 0; i < 32; i++) {
        reversed = (reversed << 1) | (x & 1u);
        x >>= 1;
    }
    return reversed;
}

static uint32_t xor_unique(const uint32_t *values, int n) {
    uint32_t result = 0;
    if (values == NULL || n < 0) return 0;
    for (int i = 0; i < n; i++) result ^= values[i];
    return result;
}

static uint32_t xor_missing(const uint32_t *values, int n) {
    uint32_t result = 0;
    if (values == NULL || n < 0) return 0;
    for (int i = 0; i <= n; i++) result ^= (uint32_t)i;
    for (int i = 0; i < n; i++) result ^= values[i];
    return result;
}

static uint64_t combine64(uint32_t high, uint32_t low) {
    return ((uint64_t)high << 32) | low;
}

int main(void) {
    assert(is_pow2(1) && is_pow2(1024) && !is_pow2(0) && !is_pow2(12));
    assert(floor_pow2(100) == 64 && floor_pow2(0) == 0);
    assert(ceil_pow2(100) == 128 && ceil_pow2(1) == 1);
    assert(ceil_pow2(0x80000001u) == 0);
    for (uint32_t value = 0; value < 100000; value += 97) {
        assert(popcount_kern(value) == popcount_swar(value));
    }
    assert(put_field(0xDEADBEEFu, 8, 8, 0xAA) == 0xDEADAAEFu);
    assert(get_field(0xDEADAAEFu, 8, 8) == 0xAA);
    assert(rol32(0x12345678u, 32) == 0x12345678u);
    assert(ror32(rol32(0x12345678u, 7), 7) == 0x12345678u);
    assert(ffs32(0x10) == 5 && ffs32(0) == 0);
    assert(clz32(1) == 31 && clz32(0) == 32);
    assert(ctz32(0x10) == 4 && ctz32(0) == 32);
    assert(bswap32(0x01020304u) == 0x04030201u);
    assert(reverse32(reverse32(0x12345678u)) == 0x12345678u);
    const uint32_t pairs[] = {4, 7, 4, 9, 9};
    const uint32_t missing[] = {0, 1, 3, 4};
    assert(xor_unique(pairs, 5) == 7 && xor_missing(missing, 4) == 2);
    assert(combine64(0xDEADBEEFu, 0x01020304u) == UINT64_C(0xDEADBEEF01020304));
    puts("s01_bitops: all assertions passed");
    return 0;
}