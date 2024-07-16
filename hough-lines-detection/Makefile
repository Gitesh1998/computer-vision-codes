# Define the directories for includes and libraries
INCLUDE_DIRS = -I/usr/include/opencv4  # Path to the OpenCV header files
LIB_DIRS = -L/usr/lib  # Path to the OpenCV libraries for linking

# Define the compiler and flags
CC = g++
CFLAGS = -O2 -g $(INCLUDE_DIRS)  # Use -O2 for optimization; -g for debugging symbols

# Define the libraries required for linking
LIBS = $(LIB_DIRS) -lopencv_core -lopencv_flann -lopencv_video -lrt  # OpenCV libraries and real-time library

# Define the target executable and object files
TARGET = hough-line-detection
OBJS = hough-line-detection.o

# Default target to build the executable
all: $(TARGET)  # Default target to build the hough-line-detection executable

# Link the object files to create the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)  # Link object files with libraries

# Compile the source file into an object file
$(OBJS): hough-line-detection.cpp
	$(CC) $(CFLAGS) -c hough-line-detection.cpp  # Compile source file to object file

# Clean up build artifacts
clean:
	rm -f $(TARGET) $(OBJS)  # Remove target and object files

# Rebuild target and object files
rebuild: clean all  # Clean and then build the project
