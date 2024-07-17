# Define the compiler
CC = g++  # C++ compiler

# Define the target executable
TARGET = object-detection  # Name of the final executable

# Define compiler flags
CFLAGS = -O2 -g -I/usr/include/opencv4  # -O2 for optimization, -g for debugging symbols, include OpenCV headers

# Define library directories and libraries to link
LIB_DIRS = -L/usr/lib  # Path to the OpenCV libraries
LIBS = $(LIB_DIRS) -lopencv_core -lopencv_flann -lopencv_video -lrt  # Link OpenCV core, Flann, Video libraries, and real-time library

# Default target to build the executable
all: $(TARGET)  # Build the 'object-detection' executable by default

# Rule for linking the executable
$(TARGET): object-detection.o  # Link object file to create the executable
	$(CC) $(CFLAGS) -o $(TARGET) object-detection.o `pkg-config --libs opencv4` $(LIBS)  # Link object file with OpenCV libraries and create the executable

# Rule for compiling the source file into an object file
object-detection.o: object-detection.cpp  # Compile the source file into an object file
	$(CC) $(CFLAGS) -c object-detection.cpp  # Compile source file with flags into object file

# Rule for cleaning up build artifacts
clean:
	rm -f $(TARGET) object-detection.o  # Remove the executable and object file; -f to ignore errors if files do not exist
