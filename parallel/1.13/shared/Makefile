CXXFLAGS	= -std=c++11 -pg -g

SHARED_BIN  = transpond.c

all: transpond

shared: $(SHARED_BIN)

transpond: transpond.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS) -pthread

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $< -pthread

clean:
	$(RM) *.o $(SHARED_BIN) gmon.out

.PHONY: all shared clean
