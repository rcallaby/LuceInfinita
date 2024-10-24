# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source files (only .cpp files)
SRC_FILES = $(SRC_DIR)/main.cpp \
            $(SRC_DIR)/Logger.cpp \
            $(SRC_DIR)/NetworkLayer.cpp \
            $(SRC_DIR)/TaskScheduler.cpp \
            $(SRC_DIR)/SessionManager.cpp \
            $(SRC_DIR)/CLI.cpp

# Object files (auto-generated based on source files)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Target executable
TARGET = $(BIN_DIR)/luceinfinita

# Default rule
all: $(TARGET)

# Create target executable by linking all object files
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile each .cpp file to a .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove object files and the executable
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run the program (optional)
run: $(TARGET)
	./$(TARGET)

# Phony targets to avoid conflicts with file names
.PHONY: all clean run
