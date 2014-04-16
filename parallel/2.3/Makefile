CXXFLAGS	= -std=c++11 -pg -g

TEE_BIN		= tee.a tee.b tee.c

all: $(TEE_BIN)

tee.a: tee.a.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

tee.b: tee.b.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS) -pthread

tee.c: tee.c.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) *.o $(TEE_BIN) gmon.out

.PHONY: all shared clean
