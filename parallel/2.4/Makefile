CXXFLAGS	= -std=c++11 -pg -g

DIFFER_BIN		= differ.a differ.b differ.c

all: $(DIFFER_BIN)

differ.a: differ.a.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

differ.b: differ.b.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS) -pthread

differ.c: differ.c.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS) -pthread

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) *.o $(DIFFER_BIN) gmon.out

.PHONY: all shared clean
