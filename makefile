# Makefile for OpenGL projects

# Compiler and flags
CC = g++
CFLAGS = -fdiagnostics-color=always -g -Wall -Wextra -std=c++17 # Adjust C++ standard as needed

# Libraries
LIBS = -lGL -lglfw

# Source files and object files
SOURCES = $(wildcard *.cpp) $(wildcard glad/*.c)
OBJECTS = $(SOURCES:.cpp=.o:.c=.o)

# Executable name
EXECUTABLE = main

# Default target
all: $(EXECUTABLE)

# Rule to create object files from source files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

%.o: glad/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to link object files into the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS)

# Clean target to remove object files and the executable
clean:
	-rm -f $(OBJECTS) $(EXECUTABLE)
# Run Target to run the executable
run: $(EXECUTABLE)
	./$(EXECUTABLE)

# Debug Target to run the executable with gdb
debug: $(EXECUTABLE)
	gdb ./$(EXECUTABLE)

# Rebuild
rebuild: clean all
