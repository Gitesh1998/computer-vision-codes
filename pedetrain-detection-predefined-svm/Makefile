# Define the compiler
CC = g++  # C++ compiler

# Define the target executable
TARGET = peopleDetect  # Name of the final executable

# Define compiler flags
CFLAGS = -O0 -g -I/usr/include/opencv4  # -O0 for no optimization, -g for debugging symbols, include OpenCV headers

# Define library directories and libraries to link
LIB_DIRS = -L/usr/lib  # Path to the OpenCV libraries
LIBS = $(LIB_DIRS) -lopencv_core -lopencv_flann -lopencv_video -lrt  # Link OpenCV core, Flann, Video libraries, and real-time library

# Default target to build the executable
all: $(TARGET)  # Build the 'peopleDetect' executable by default

# Rule for linking the executable
$(TARGET): peopleDetect.o  # Link object file to create the executable
	$(CC) $(CFLAGS) -o $(TARGET) peopleDetect.o `pkg-config --libs opencv4` $(LIBS)  # Link object file with OpenCV libraries and create the executable

# Rule for compiling the source file into an object file
peopleDetect.o: peopleDetect.cpp  # Compile the source file into an object file
	$(CC) $(CFLAGS) -c peopleDetect.cpp  # Compile source file with flags into object file

# Rule for cleaning up build artifacts
clean:
	rm -f $(TARGET) peopleDetect.o  # Remove the executable and object file; -f to ignore errors if files do not exist
