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
    /* TODO: combine core, PCIe/LTR, USB, display, and memory-controller votes. */
    (void)platform;
    if (why != NULL && why_size != 0) why[0] = '\0';
    return PKG_C0;
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
    puts("Complete the TODO to pass s17_pkgc_blocker_arbiter.");
    return 0;
}