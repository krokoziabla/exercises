CFLAGS      = -std=c11 -pg

all: sequential parallel

sequential: sequential.o
	$(CC) $(CFLAGS) -o $@ $^

parallel: parallel.o
	$(CC) $(CFLAGS) -o $@ $^ -pthread

.PHONY: all
