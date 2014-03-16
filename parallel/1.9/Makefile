CFLAGS      = -std=c11 -pg -g
LDFLAGS     = -lm

all: quick_seq quick_par

quick_seq: quick_seq.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

quick_par: quick_par.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -pthread

.PHONY: all
