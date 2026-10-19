#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define SETS 8
#define WAYS 4
#define LINE_SIZE 64

typedef struct { bool valid; uint32_t tag; unsigned age; } line_t;
typedef struct { line_t line[SETS][WAYS]; unsigned clock; unsigned hits, misses; } cache_t;

static bool access(cache_t *cache, uint32_t addr) {
    /* TODO: decode tag/set/offset, update LRU state, and count hits/misses. */
    (void)cache;
    (void)addr;
    return false;
}

int main(void) {
    cache_t sequential = {0}, stride = {0};
    for (uint32_t i = 0; i < 64; i++) {
        (void)access(&sequential, i * 4);
        (void)access(&stride, i * 1024);
    }
    assert(sequential.hits == 60 && sequential.misses == 4);
    assert(stride.hits == 0 && stride.misses == 64);
    puts("Complete the TODO to pass s08_cache_sim.");
    return 0;
}