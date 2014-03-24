CFLAGS      = -std=c11 -pg -g

SHARED_BIN  = transpond.c

all: transpond worker

shared: $(SHARED_BIN)

transpond: transpond.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

worker: worker.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) *.o $(SHARED_BIN) gmon.out

.PHONY: all shared clean
