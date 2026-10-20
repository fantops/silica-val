#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define TLB_ENTRIES 4
#define IO_L1_ENTRIES 16
#define IO_L2_ENTRIES 256

typedef struct { uint32_t vpn, pfn; bool valid; } tlb_entry_t;
typedef struct { uint32_t pfn; bool present, writable; } io_pte_t;
typedef struct { io_pte_t pt[IO_L2_ENTRIES]; bool present; } io_pd_t;
typedef struct { uint16_t bdf; io_pd_t *pd[IO_L1_ENTRIES]; } iommu_ctx_t;

static tlb_entry_t tlb[TLB_ENTRIES];
static unsigned evict_next;
static unsigned hits, misses;

static uint32_t translate(uint32_t va) {
    /* TODO: FIFO TLB lookup; on miss, derive a PFN and install it. */
    (void)va;
    return 0;
}

static bool iommu_translate(iommu_ctx_t *ctx, uint32_t iova, bool write, uint64_t *pa, const char **fault) {
    /* TODO: walk both levels; distinguish missing entries and permission faults. */
    (void)ctx;
    (void)iova;
    (void)write;
    (void)pa;
    (void)fault;
    return false;
}

int main(void) {
    uint32_t first = translate(0x1234), second = translate(0x1234);
    assert(first == second && hits == 1 && misses == 1);
    io_pd_t directory = {.present = true};
    iommu_ctx_t ctx = {.bdf = 0x1200};
    ctx.pd[0] = &directory;
    uint64_t pa = 0;
    const char *fault = NULL;
    directory.pt[1] = (io_pte_t){.pfn = 0xabc, .present = true, .writable = false};
    assert(iommu_translate(&ctx, 0x1000, false, &pa, &fault) && pa == 0xabc000);
    assert(!iommu_translate(&ctx, 0x2000, false, &pa, &fault) && fault != NULL);
    assert(!iommu_translate(&ctx, 0x1000, true, &pa, &fault) && fault != NULL);
    assert(!iommu_translate(&ctx, 0x100000, false, &pa, &fault) && fault != NULL);
    pa = UINT64_C(0xdeadbeef);
    assert(!iommu_translate(&ctx, 0x01001000, false, &pa, &fault));
    assert(pa == UINT64_C(0xdeadbeef) && fault != NULL);
    for (unsigned slot = 0; slot < TLB_ENTRIES; slot++) tlb[slot].valid = false;
    evict_next = 0;
    for (uint32_t page = 0; page < TLB_ENTRIES + 1; page++) (void)translate(page << 12);
    unsigned previous_misses = misses;
    (void)translate(0);
    assert(misses == previous_misses + 1);
    puts("s13_tlb_iommu: all assertions passed");
    return 0;
}
