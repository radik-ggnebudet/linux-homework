CXX = g++
CXXFLAGS = -Wall -std=c++11

OBJS = main.o complex.o sort.o

all: program

program: $(OBJS)
	$(CXX) $(CXXFLAGS) -o program $(OBJS)

main.o: main.cpp complex.h sort.h
	$(CXX) $(CXXFLAGS) -c main.cpp

complex.o: complex.cpp complex.h
	$(CXX) $(CXXFLAGS) -c complex.cpp

sort.o: sort.cpp sort.h complex.h
	$(CXX) $(CXXFLAGS) -c sort.cpp

clean:
	rm -f *.o program
