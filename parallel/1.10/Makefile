CFLAGS      = -std=c11 -pg

all: pipe_overflow

pipe_overflow: pipe_overflow.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

pipe_deadlock: pipe_deadlock.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all
