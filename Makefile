CXX=g++

INC=-I.

SOURCES=pq.cpp graph.cpp hexcell.cpp hexboard.cpp hex.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hex

CXXFLAGS=-O3 -std=c++11 $(INC)

all: play

play: $(SOURCES) $(EXECUTABLE)
	./$(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.o: 
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf *o *out $(EXECUTABLE)
