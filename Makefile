# Compiler
CXX = /usr/bin/i686-w64-mingw32-g++
CXXFLAGS = -MMD -MP -fdiagnostics-color=always -fno-exceptions -Wextra \
	-Wall -pedantic -Wconversion -Wnull-dereference -O0 -march=native -std=c++20 -g -I . \
	-I 'prefix/drive_c/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/core/inc' \
	-L'prefix/drive_c/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/core/lib/x86' \
	-lfmod -lgdi32 -ld3d9 -ld3dx9 -ldinput8 -ldxguid -static-libgcc -static-libstdc++ -static

SRC_DIRS := audio direct3d engine game input physics ui
TARGET_DIR := out
TARGET := $(TARGET_DIR)/game     # final binary name

# ==== BUILD LOGIC ====
# Collect all .cpp files from SRC_DIRS
SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))

# Generate object and dependency file paths in TARGET_DIR
OBJS := $(patsubst %.cpp,$(TARGET_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

# Default target
all: $(TARGET)

# Link step
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
# 	$(CXX) $(CXXFLAGS) $(OBJS) -v -o $@

# Compile step (pattern rule)
$(TARGET_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Include dependency files if they exist
-include $(DEPS)

# Cleanup
clean:
	rm -rf $(TARGET_DIR)

.PHONY: all clean
