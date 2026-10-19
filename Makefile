# Unified coding workspace for the 18-session prep.
#
# Layout:
#   exercises/   YOU write code here.   Files: dNN_<name>.c or sNN_<name>.c
#   solutions/   reference implementations (peek only after attempting).
#
# Targets:
#   make ex-<name>     build + run YOUR exercise (e.g. make ex-s01_bitops)
#   make sol-<name>    build + run the reference solution
#   make all-ex        build every exercise starter
#   make all-sol       build every reference solution
#   make clean

CC      = clang
CFLAGS  = -Wall -Wextra -Wpedantic -std=c11 -g -O0 -I.
BIN     = bin

EX_SRCS  = $(sort $(wildcard exercises/d[0-9][0-9]_*.c exercises/s[0-9][0-9]_*.c))
# d00_csr_init.c, d00_csr_model.c are library helpers (no main()).
# d00_bench.c references their externs, so it isn't built by default either.
# `make sol-d00_bench` is opt-in if you wire it up.
SOL_SRCS = $(filter-out solutions/d00_bench.c solutions/d00_csr_init.c solutions/d00_csr_model.c,$(sort $(wildcard solutions/d[0-9][0-9]_*.c solutions/s[0-9][0-9]_*.c)))

EX_BINS  = $(patsubst exercises/%.c,$(BIN)/ex-%,$(EX_SRCS))
SOL_BINS = $(patsubst solutions/%.c,$(BIN)/sol-%,$(SOL_SRCS))

all-sol: $(SOL_BINS)
all-ex:  $(EX_BINS)

$(BIN):
	mkdir -p $(BIN)

$(BIN)/ex-%: exercises/%.c | $(BIN)
	$(CC) $(CFLAGS) -o $@ $<

$(BIN)/sol-%: solutions/%.c | $(BIN)
	$(CC) $(CFLAGS) -o $@ $<

ex-%: $(BIN)/ex-%
	./$<

sol-%: $(BIN)/sol-%
	./$<

clean:
	rm -rf $(BIN)

.PHONY: all-sol all-ex clean
