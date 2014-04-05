CXXFLAGS	= -std=c++11 -pg -g

GREP_BIN  = grep.cc

all: grep

shared: $(GREP_BIN)

grep: grep.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS) -pthread

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $< -pthread

clean:
	$(RM) *.o $(GREP_BIN) gmon.out

.PHONY: all shared clean
