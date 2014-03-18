CFLAGS      = -std=c11 -pg -g

RING_BIN    = ring_controller \
              ring_worker

all: ring

ring: $(RING_BIN)

ring_controller: ring_controller.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

ring_worker: ring_worker.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) *.o $(RING_BIN) gmon.out

.PHONY: all ring clean
