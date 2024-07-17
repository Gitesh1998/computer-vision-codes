# Define the compiler
CC = g++  # C++ compiler

# Define directories for header files and libraries
INCLUDE_DIRS = -I/usr/include/opencv4  # Path to OpenCV header files
LIB_DIRS = -L/usr/lib  # Path to OpenCV libraries

# Define compiler flags
CDEFS =  # Additional compiler definitions (empty for now)
CFLAGS = -O0 -g $(INCLUDE_DIRS) $(CDEFS)  # -O0 for no optimization, -g for debugging symbols, include OpenCV headers

# Define libraries to link
LIBS = $(LIB_DIRS) -lopencv_core -lopencv_flann -lopencv_video -lrt  # Link OpenCV core, Flann, Video libraries, and real-time library

# Default target to build the executable
all: skeletal  # Build the 'skeletal' executable by default

# Rule for linking the final executable
skeletal: skeletal.o  # Link object file to create the executable
	$(CC) $(CFLAGS) -o $@ $^ `pkg-config --libs opencv4` $(LIBS)  # Link object file with OpenCV libraries

# Rule for compiling the source file into an object file
skeletal.o: skeletal.cpp  # Compile the source file into an object file
	$(CC) $(CFLAGS) -c $<  # Compile source file with flags into object file

# Rule for cleaning up build artifacts
clean:
	rm -f skeletal skeletal.o *.pgm  # Remove the executable, object file, and PGM files
