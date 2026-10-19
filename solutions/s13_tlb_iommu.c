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
    uint32_t vpn = va >> 12;
    for (unsigned i = 0; i < TLB_ENTRIES; i++) {
        if (tlb[i].valid && tlb[i].vpn == vpn) { hits++; return (tlb[i].pfn << 12) | (va & 0xfffu); }
    }
    misses++;
    uint32_t pfn = (vpn * 2654435761u) & 0xfffffu;
    tlb[evict_next] = (tlb_entry_t){.vpn = vpn, .pfn = pfn, .valid = true};
    evict_next = (evict_next + 1) % TLB_ENTRIES;
    return (pfn << 12) | (va & 0xfffu);
}

static bool iommu_translate(iommu_ctx_t *ctx, uint32_t iova, bool write, uint64_t *pa, const char **fault) {
    unsigned l1 = (iova >> 20) & 0xfu;
    unsigned l2 = (iova >> 12) & 0xffu;
    if (ctx->pd[l1] == NULL || !ctx->pd[l1]->present) { *fault = "L1 not present"; return false; }
    io_pte_t *pte = &ctx->pd[l1]->pt[l2];
    if (!pte->present) { *fault = "L2 not present"; return false; }
    if (write && !pte->writable) { *fault = "write to read-only page"; return false; }
    *pa = ((uint64_t)pte->pfn << 12) | (iova & 0xfffu);
    *fault = NULL;
    return true;
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
    puts("s13_tlb_iommu: all assertions passed");
    return 0;
}