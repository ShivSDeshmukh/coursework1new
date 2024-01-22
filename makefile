# Makefile

CC = g++  
CFLAGS = -Wall -std=c++11  
SRC_DIR = src
BUILD_DIR = build
TARGET = oop.cpp

# List all source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Generate corresponding object file names
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Default target
all: $(BUILD_DIR) $(TARGET)

# Rule to make the executable
$(TARGET): $(OBJS)
    $(CC) $(CFLAGS) -o $@ $^

# Rule to compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
    $(CC) $(CFLAGS) -c -o $@ $<

# Rule to create the build directory
$(BUILD_DIR):
    mkdir -p $(BUILD_DIR)

# Clean up intermediate and executable files
clean:
    rm -rf $(BUILD_DIR) $(TARGET)
