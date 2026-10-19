#include <assert.h>
#include <stdio.h>

static int clk_mux_step(int *selected, int sel, int clk_a, int clk_b, int *out, int *runt) {
    /* TODO: switch sources only when both clocks are safely low. */
    (void)selected;
    (void)sel;
    (void)clk_a;
    (void)clk_b;
    (void)out;
    (void)runt;
    return -1;
}

static int div_n(int *count, int clk_in, int n) {
    /* TODO: advance on rising edges and toggle the divided clock every N edges. */
    (void)count;
    (void)clk_in;
    (void)n;
    return -1;
}

static int reset_seq(int *dom_rst, const int *order, int n, int step, int *bad_order) {
    /* TODO: deassert one reset per step and reject invalid/duplicate domain order. */
    (void)dom_rst;
    (void)order;
    (void)n;
    (void)step;
    (void)bad_order;
    return -1;
}

int main(void) {
    int selected = 0, out = 0, runt = 0;
    assert(clk_mux_step(&selected, 1, 0, 0, &out, &runt) == 0);
    assert(selected == 1 && runt == 0);
    int count = 0;
    assert(div_n(&count, 1, 2) == 0);
    (void)div_n(&count, 0, 2);
    assert(div_n(&count, 1, 2) == 1);
    int resets[] = {1, 1, 1};
    const int order[] = {0, 1, 2};
    int bad = 0;
    assert(reset_seq(resets, order, 3, 0, &bad) == 1 && !bad && resets[0] == 0);
    assert(reset_seq(resets, order, 3, 1, &bad) == 2 && resets[1] == 0);
    puts("Complete the TODOs to pass s03_clock_reset.");
    return 0;
}