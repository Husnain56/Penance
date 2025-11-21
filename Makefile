.PHONY: all game clean rebuild help

# --- Configuration ---
BUILD_DIR := build
EXECUTABLE := $(BUILD_DIR)/Penance
BUILD_TYPE := Debug
CORES := $(shell nproc)  # Auto-detects your CPU cores for faster builds

# Default target
all: game

# --- Main Flow ---
game:
	@echo "=== 1. Configuring CMake (Linux) ==="
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

	@echo "=== 2. Building Project (Using $(CORES) cores) ==="
	cmake --build $(BUILD_DIR) --parallel $(CORES)

	@echo "=== 3. Running Game ==="
	$(EXECUTABLE)

# --- Utilities ---
clean:
	@echo "Removing build directory..."
	rm -rf $(BUILD_DIR)

rebuild: clean game

help:
	@echo "Linux Makefile for Penance"
	@echo "  make         : Configure, build, and run the game"
	@echo "  make clean   : Delete the build folder"
	@echo "  make rebuild : Delete build folder and start fresh"
