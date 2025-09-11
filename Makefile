# Compiler
CXX = /usr/bin/i686-w64-mingw32-g++
CXXFLAGS = -MMD -MP -fdiagnostics-color=always -fno-exceptions -Wextra \
	-Wall -pedantic -Wconversion -Wnull-dereference -O0 -march=native -std=c++20 -g \
	-I '/home/wilson/tarc/game/prefix/drive_c/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/core/inc' \
	-L'/home/wilson/tarc/game/prefix/drive_c/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/core/lib/x86' \
	-lfmod -lgdi32 -ld3d9 -ld3dx9 -ldinput8 -ldxguid -static-libgcc -static-libstdc++ -static

# Directories
SRC_DIR = .
BUILD_DIR = out
BIN_DIR = .

# Target executable
TARGET = $(BIN_DIR)/main

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files in build directory
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

DEPS = $(OBJS:.o=.d)

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS) | $(BIN_DIR)
# 	@$(CXX) $(CXXFLAGS) -o $@ $^
	@echo

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build and bin directories if they don't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean up object files and executable
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Optional: rebuild
rebuild: clean all

-include $(DEPS)
