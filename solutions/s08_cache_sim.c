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
    unsigned line = addr / LINE_SIZE;
    unsigned set = line % SETS;
    uint32_t tag = line / SETS;
    cache->clock++;
    for (unsigned way = 0; way < WAYS; way++) {
        line_t *entry = &cache->line[set][way];
        if (entry->valid && entry->tag == tag) {
            entry->age = cache->clock;
            cache->hits++;
            return true;
        }
    }
    unsigned victim = 0;
    for (unsigned way = 0; way < WAYS; way++) {
        if (!cache->line[set][way].valid) { victim = way; break; }
        if (cache->line[set][way].age < cache->line[set][victim].age) victim = way;
    }
    cache->line[set][victim] = (line_t){.valid = true, .tag = tag, .age = cache->clock};
    cache->misses++;
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
    cache_t replacement = {0};
    for (uint32_t line = 0; line < WAYS; line++) assert(!access(&replacement, line * SETS * LINE_SIZE));
    assert(access(&replacement, 0));
    assert(!access(&replacement, WAYS * SETS * LINE_SIZE));
    assert(access(&replacement, 0));
    assert(!access(&replacement, SETS * LINE_SIZE));
    printf("s08_cache_sim: sequential %u/%u hits, stride %u/%u hits\n",
           sequential.hits, sequential.misses, stride.hits, stride.misses);
    return 0;
}