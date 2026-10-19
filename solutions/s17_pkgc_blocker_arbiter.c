#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef enum { ASPM_L0, ASPM_L0S, ASPM_L1, ASPM_L1_1, ASPM_L1_2 } aspm_t;
typedef enum { PKG_C0 = 0, PKG_C2, PKG_C6, PKG_C10 } pkg_state_t;
typedef struct { const char *bdf; aspm_t state; int ltr_us; } pcie_vote_t;
typedef struct {
    int n_cores;
    int core_cs[8];
    int n_links;
    pcie_vote_t links[16];
    bool usb_idle, display_idle, mc_self_refresh;
} platform_t;

static pkg_state_t arbitrate(const platform_t *platform, char *why, size_t why_size) {
    if (platform == NULL || why == NULL || why_size == 0) return PKG_C0;
    why[0] = '\0';
    if (platform->n_cores < 0 || platform->n_cores > 8 || platform->n_links < 0 || platform->n_links > 16) {
        snprintf(why, why_size, "invalid vote count");
        return PKG_C0;
    }
    for (int i = 0; i < platform->n_cores; i++) {
        if (platform->core_cs[i] < 6) {
            snprintf(why, why_size, "core %d is not in C6", i);
            return PKG_C2;
        }
    }
    if (!platform->usb_idle) { snprintf(why, why_size, "USB controller not idle"); return PKG_C2; }
    if (!platform->display_idle) { snprintf(why, why_size, "display engine not idle"); return PKG_C2; }
    if (!platform->mc_self_refresh) { snprintf(why, why_size, "memory controller not in self-refresh"); return PKG_C2; }
    for (int i = 0; i < platform->n_links; i++) {
        const pcie_vote_t *link = &platform->links[i];
        if (link->state != ASPM_L1_2 || link->ltr_us < 1000) {
            snprintf(why, why_size, "PCIe link %s state=%d LTR=%d us", link->bdf,
                     (int)link->state, link->ltr_us);
            return PKG_C6;
        }
    }
    snprintf(why, why_size, "all package-idle votes satisfied");
    return PKG_C10;
}

int main(void) {
    platform_t platform = {
        .n_cores = 2, .core_cs = {6, 6}, .n_links = 1,
        .links = {{.bdf = "03:00.0", .state = ASPM_L1_2, .ltr_us = 1500}},
        .usb_idle = true, .display_idle = true, .mc_self_refresh = true
    };
    char why[128];
    assert(arbitrate(&platform, why, sizeof(why)) == PKG_C10);
    platform.links[0].state = ASPM_L1;
    platform.links[0].ltr_us = 10;
    assert(arbitrate(&platform, why, sizeof(why)) == PKG_C6);
    assert(why[0] != '\0');
    platform.links[0].state = ASPM_L1_2;
    platform.links[0].ltr_us = 1500;
    platform.usb_idle = false;
    assert(arbitrate(&platform, why, sizeof(why)) == PKG_C2);
    puts("s17_pkgc_blocker_arbiter: all assertions passed");
    return 0;
}