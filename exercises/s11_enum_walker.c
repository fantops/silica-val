#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_NODES 8
typedef struct {
    uint16_t vid, did;
    uint8_t header_type, secondary_bus;
    uint32_t bar[6];
    uint64_t window_base;
    bool msi, msix;
} func_t;
typedef struct { uint8_t bus, dev, fn; func_t f; } node_t;
typedef struct { node_t *nodes; int count; } tree_t;

static node_t discovered[MAX_NODES];
static node_t inventory[] = {
    {.bus = 0, .dev = 0, .fn = 0, .f = {.vid = 0x8086, .did = 0x1000, .header_type = 1}},
    {.bus = 0, .dev = 1, .fn = 0, .f = {.vid = 0x1234, .did = 0x0001, .header_type = 1, .secondary_bus = 1}},
    {.bus = 1, .dev = 0, .fn = 0, .f = {.vid = 0x1a2b, .did = 0x0002, .bar = {0xFFF00000}, .msi = true}},
    {.bus = 1, .dev = 1, .fn = 0, .f = {.vid = 0x1a2b, .did = 0x0003, .bar = {0xFFFFF000}, .msix = true}}
};

static uint64_t bar_size(uint32_t readback) {
    /* TODO: derive the power-of-two BAR size from the readback mask. */
    (void)readback;
    return 0;
}

static void enumerate(uint8_t bus, tree_t *out) {
    /* TODO: scan the fake topology depth-first and recurse through bridges. */
    (void)bus;
    (void)out;
}

static void program_bridge_window(node_t *bridge, uint64_t mem_base) {
    /* TODO: assign an aligned memory window to the bridge. */
    (void)bridge;
    (void)mem_base;
}

int main(void) {
    tree_t tree = {.nodes = discovered, .count = 0};
    enumerate(0, &tree);
    assert(tree.count == 4);
    assert(bar_size(0xFFF00004u) == 0x100000u);
    program_bridge_window(&tree.nodes[1], 0x80000000u);
    assert(tree.nodes[1].f.window_base == 0x80000000u);
    program_bridge_window(&tree.nodes[1], 0x80000001u);
    assert(tree.nodes[1].f.window_base == 0x80100000u);
    node_t alternate[MAX_NODES];
    tree_t independent = {.nodes = alternate, .count = 0};
    enumerate(0, &independent);
    assert(independent.count == 4 && alternate[3].f.msix);
    assert(tree.nodes[3].f.msix);
    puts("s11_enum_walker: 4 functions discovered; all assertions passed");
    return 0;
}
