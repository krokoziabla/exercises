CFLAGS      = -std=c11 -pg -g

STAR_BIN    = star_controller \
              star_worker

RING_BIN    = ring_controller \
              ring_worker

all: star ring

star: $(STAR_BIN)

star_controller: star_controller.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

star_worker: star_worker.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

ring: $(RING_BIN)

ring_controller: ring_controller.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

ring_worker: ring_worker.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) *.o $(STAR_BIN) gmon.out

.PHONY: all star ring clean
