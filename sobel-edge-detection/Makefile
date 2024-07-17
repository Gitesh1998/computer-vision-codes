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
all: sobel_edge_detection  # Build the 'sobel_edge_detection' executable by default

# Rule for linking the final executable
sobel_edge_detection: sobel_edge_detection.o  # Link object file to create the executable
	$(CC) $(CFLAGS) -o $@ $^ `pkg-config --libs opencv4` $(LIBS)  # Link object file with OpenCV libraries and additional libraries

# Rule for compiling the source file into an object file
sobel_edge_detection.o: sobel_edge_detection.cpp  # Compile the source file into an object file
	$(CC) $(CFLAGS) -c $<  # Compile source file with flags into object file

# Rule for cleaning up build artifacts
clean:
	rm -f sobel_edge_detection sobel_edge_detection.o  # Remove the executable and object file
