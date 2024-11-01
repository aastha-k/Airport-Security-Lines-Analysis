# Makefile

CXX = g++
CXXFLAGS = -std=c++11 -pthread

SRCS = main.cpp Passenger.cpp SingleServerInfiniteBuffer.cpp SingleServerFiniteBuffer.cpp MultiServerInfiniteBuffer.cpp MultiServerFiniteBuffer.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = Passenger.h Globals.h Simulations.h

all: simulation

simulation: $(OBJS)
	$(CXX) $(CXXFLAGS) -o simulation $(OBJS)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o simulation
