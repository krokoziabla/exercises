CFLAGS      = -std=c11 -pg

sequential: sequential.o
	$(CC) $(CFLAGS) -o $@ $^

parallel: parallel.o
	$(CC) $(CFLAGS) -o $@ $^
