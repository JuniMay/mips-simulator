CC ?= gcc
SRCDIR ?= ./src

CFLAGS ?= -Wall -g -I$(SRCDIR)
CFLAGS += -O2

sim: $(SRCDIR)/shell.c $(SRCDIR)/sim.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf *.o *~ sim
