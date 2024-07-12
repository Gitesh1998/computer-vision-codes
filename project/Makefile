# Define the compiler
CC = g++  # C++ compiler

# Define the directories for header files and libraries
INCLUDE_DIRS = -I/usr/include/opencv4  # Path to OpenCV header files
LIB_DIRS = -L/usr/lib  # Path to OpenCV libraries

# Define compiler flags
CDEFS =  # Additional compiler definitions (empty for now)
CFLAGS = -O3 -g $(INCLUDE_DIRS) $(CDEFS)  # -O3 for high optimization, -g for debugging symbols, include OpenCV headers

# Define libraries to link
LIBS = $(LIB_DIRS) -lopencv_core -lopencv_flann -lopencv_video -lrt  # Link OpenCV core, Flann, Video libraries, and real-time library

# Default target to build the executable
all: main  # Build the 'main' executable by default

# Rule for linking the final executable
main: main.o  # Link object file to create the executable
	$(CC) $(CFLAGS) -o $@ $^ -fopenmp `pkg-config --libs opencv4` $(LIBS)  # Link object file with OpenCV libraries and enable OpenMP support

# Rule for compiling the source file into an object file
main.o: main.cpp  # Compile the source file into an object file
	$(CC) $(CFLAGS) -c $< -fopenmp  # Compile source file with flags into object file and enable OpenMP support

# Rule for cleaning up build artifacts
clean:
	rm -f main main.o output*.avi output*.mp4  # Remove the executable, object file, and video files
