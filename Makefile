CXX=g++

INC=-I.

SOURCES=pq.cpp graph.cpp hexcell.cpp hexboard.cpp hex.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hex

CXXFLAGS=-O3 -std=c++11 $(INC)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.c.o: 
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: all
	./$(EXECUTABLE)


clean:
	rm -rf *o *out $(EXECUTABLE)
