#include <assert.h>
#include <stdio.h>

static int clk_mux_step(int *selected, int sel, int clk_a, int clk_b, int *out, int *runt) {
    if (selected == NULL || out == NULL || runt == NULL) return -1;
    *runt = 0;
    if ((sel == 0 || sel == 1) && sel != *selected && !clk_a && !clk_b) *selected = sel;
    *out = *selected ? clk_b : clk_a;
    return *out;
}

typedef struct { unsigned count; int previous_clock, output; } divider_t;

static int div_n(divider_t *divider, int clk_in, int n) {
    if (divider == NULL || n < 2 || n % 2 != 0) return -1;
    if (clk_in && !divider->previous_clock) {
        divider->count++;
        if (divider->count >= (unsigned)n / 2) {
            divider->count = 0;
            divider->output = !divider->output;
        }
    }
    divider->previous_clock = clk_in;
    return divider->output;
}

static int reset_seq(int *dom_rst, const int *order, int n, int step, int *bad_order) {
    if (dom_rst == NULL || order == NULL || bad_order == NULL || n < 0 || step < 0 || step >= n) return -1;
    *bad_order = 0;
    for (int i = 0; i < n; i++) {
        if (order[i] < 0 || order[i] >= n) *bad_order = 1;
        for (int j = 0; j < i; j++) if (order[i] == order[j]) *bad_order = 1;
    }
    if (*bad_order) return -1;
    for (int prior = 0; prior < step; prior++) {
        if (dom_rst[order[prior]] != 0) {
            *bad_order = 1;
            return -1;
        }
    }
    dom_rst[order[step]] = 0;
    return step + 1;
}

int main(void) {
    int selected = 0, out = 0, runt = 0;
    assert(clk_mux_step(&selected, 1, 0, 0, &out, &runt) == 0);
    assert(selected == 1 && runt == 0);
    assert(clk_mux_step(&selected, 0, 1, 1, &out, &runt) == 1 && selected == 1);
    divider_t divider = {0}, independent = {0};
    const int divide_by_four[] = {0, 1, 1, 0, 0, 1, 1, 0};
    for (unsigned edge = 0; edge < 8; edge++) {
        (void)div_n(&divider, 0, 4);
        assert(div_n(&divider, 1, 4) == divide_by_four[edge]);
        assert(div_n(&divider, 1, 4) == divide_by_four[edge]);
    }
    assert(div_n(&independent, 1, 2) == 1);
    assert(div_n(&independent, 0, 3) == -1);
    int resets[] = {1, 1, 1};
    const int order[] = {0, 1, 2};
    int bad = 0;
    assert(reset_seq(resets, order, 3, 1, &bad) == -1 && bad);
    assert(reset_seq(resets, order, 3, 0, &bad) == 1 && !bad && resets[0] == 0);
    assert(reset_seq(resets, order, 3, 1, &bad) == 2 && resets[1] == 0);
    const int duplicate[] = {0, 0, 2};
    assert(reset_seq(resets, duplicate, 3, 2, &bad) == -1 && bad);
    puts("s03_clock_reset: all assertions passed");
    return 0;
}