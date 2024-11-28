# Variables
PROJECT = particle-system
CXX = g++
CXXFLAGS = -Wc++11-extensions -g `sdl2-config --cflags` -I/opt/homebrew/include
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_gfx -lSDL2_ttf -L/opt/homebrew/lib

# Targets and dependencies
all: main

main: main.o
	$(CXX) $(CXXFLAGS) -o $(PROJECT) main.o $(LDFLAGS)

# Pattern rule for compiling .cpp files to .o files
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Special case for main.o since it does not have a corresponding header file
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp  -o main.o

clean:
	rm -f $(PROJECT) main.o foo.o

.PHONY: clean

