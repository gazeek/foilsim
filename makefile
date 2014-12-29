CC=g++
LIBS=-lncurses
CFLAGS=-Wall
SOURCES:=$(wildcard ./*.cpp)
EXECUTABLE=blob	
OBJ_DIR=obj
OBJECTS=$(SOURCES:.cpp=.o)
OBJECTS:=$(addprefix ./$(OBJ_DIR)/,$(OBJECTS))

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $@

$(OBJECTS): ./obj/%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(LIBS) -o $@

#./obj/blob.o: blob.cpp
#	$(CC) $(CFLAGS) -c blob.cpp -o ./obj/blob.o $(LIBS)

#./obj/Box.o: Box.cpp
#	$(CC) $(CFLAGS) -c Box.cpp -o ./obj/Box.o 

#./obj/Ball.o: Ball.cpp
#	$(CC) $(CFLAGS) -c Ball.cpp -o ./obj/Ball.o 
clean:
	rm -rf ./obj/*o blob
