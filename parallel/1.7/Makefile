CFLAGS      = -std=c11 -pg
LDFLAGS     = -lm

all: seq_iter seq_rec par_iter par_req

seq_iter: seq_iter.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

seq_rec: seq_rec.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

par_iter: par_iter.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -pthread

par_rec: par_rec.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -pthread

.PHONY: all
