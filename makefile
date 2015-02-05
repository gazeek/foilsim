CC=g++
LIBS=-lncurses
CFLAGS=-Wall
SOURCES_DIR=src
SOURCES:=$(wildcard ./$(SOURCES_DIR)/*.cpp)
EXECUTABLE=blob	
OBJ_DIR=obj

OBJECTS=$(SOURCES:.cpp=.o)
OBJECTS:=$(notdir $(OBJECTS))
OBJECTS:=$(addprefix ./$(OBJ_DIR)/,$(OBJECTS))

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $@

$(OBJECTS): ./$(OBJ_DIR)/%.o : ./$(SOURCES_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< $(LIBS) -o $@

#./obj/blob.o: blob.cpp
#	$(CC) $(CFLAGS) -c ./src/blob.cpp -o ./obj/blob.o $(LIBS)

#./obj/Box.o: Box.cpp
#	$(CC) $(CFLAGS) -c ./src/Box.cpp -o ./obj/Box.o 

#./obj/Ball.o: Ball.cpp
#	$(CC) $(CFLAGS) -c ./src/Ball.cpp -o ./obj/Ball.o 

clean:
	rm -rf ./obj/*o blob
