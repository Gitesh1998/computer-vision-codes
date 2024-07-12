# Define the directories for includes and libraries
INCLUDE_DIRS = -I/usr/include/opencv4
LIB_DIRS = -L/usr/lib  # Add the library directory here
LIBS = -lopencv_core -lopencv_flann -lopencv_video -lrt  # List the libraries required for linking

# Define the compiler and flags
CC = g++
CFLAGS = -O2 -g $(INCLUDE_DIRS)  # Use -O2 for optimization and keep -g for debugging; removed CDEFS as it's empty

# Define the target executable and object files
TARGET = hough-circle-detection
OBJS = hough-circle-detection.o

# Default target to build the executable
all: $(TARGET)

# Link the object files to create the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIB_DIRS) $(LIBS)  # Added $(LIB_DIRS) to the linking command

# Compile the source file into an object file
$(OBJS): hough-circle-detection.cpp
	$(CC) $(CFLAGS) -c hough-circle-detection.cpp

# Clean up build artifacts
clean:
	rm -f $(TARGET) $(OBJS)  # Added -f to avoid errors if files don't exist
