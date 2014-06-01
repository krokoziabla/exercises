CFLAGS	= -std=c11 -pg -g

BIN			= sequential parallel

all: $(BIN)

sequential: sequential.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) *.o $(BIN) gmon.out

.PHONY: all clean
