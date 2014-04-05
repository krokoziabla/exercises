CXXFLAGS	= -std=c++11 -pg -g

all: prompt

prompt: prompt.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) *.o gmon.out

.PHONY: all clean
