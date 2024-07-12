# Directory for include files
INCLUDE_DIRS = -I/usr/include/opencv4

# Directory for library files
LIB_DIRS = -L/usr/lib

# Compiler to use
CC = g++

# Compiler definitions, currently empty
CDEFS = 

# Compiler flags for optimization and debugging, including include directories
CFLAGS = -O2 -g $(INCLUDE_DIRS) $(CDEFS)

# Libraries to link against, using pkg-config to get OpenCV libraries
LIBS = $(LIB_DIRS) `pkg-config --libs opencv4` -lopencv_core -lopencv_flann -lopencv_video -lrt

# Name of the target executable
TARGET = canny_edge_detection

# Source files
SRCS = canny_edge_detection.cpp

# Object files, derived from the source files
OBJS = $(SRCS:.cpp=.o)

# Default target to build the executable
all: $(TARGET)

# Rule to link the object files into the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Pattern rule to compile .cpp files into .o files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove the target and object files
clean:
	rm -f $(TARGET) $(OBJS)
