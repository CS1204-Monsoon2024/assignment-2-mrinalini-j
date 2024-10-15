# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall

# Target executable
TARGET = main.out

# Source files
SOURCES = main.cpp HashTable.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Rule to build the executable
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Rule to compile each .cpp file into a .o file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean command to remove object files and executable
clean:
	rm -f $(OBJECTS) $(TARGET)

# Phony targets to avoid conflicts with files named "clean" or "all"
.PHONY: clean all
