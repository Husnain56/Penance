.PHONY: all game clean rebuild help

# --- OS Detection & Configuration ---
ifeq ($(OS),Windows_NT)
    # Windows Settings
    DETECTED_OS := Windows
    BUILD_DIR   := build
    # Visual Studio (MSVC) places the executable in a config subfolder
    EXECUTABLE  := ./build/Debug/Penance.exe

    # MSVC requires config to be specified at build time, not just generation
    BUILD_FLAGS := --config Debug --parallel

    # Windows equivalent of nproc is usually %NUMBER_OF_PROCESSORS%,
    # but --parallel without args defaults to all cores in modern CMake.
else
    # Linux Settings
    DETECTED_OS := Linux
    BUILD_DIR   := build
    EXECUTABLE  := ./build/Penance

    # Linux handles config at generation time
    BUILD_FLAGS := --parallel $(shell nproc)
endif

# Default target
all: game

# --- Main Flow ---
game:
	@echo "=== 1. Configuring CMake ($(DETECTED_OS)) ==="
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug

	@echo "=== 2. Building Project ==="
	cmake --build $(BUILD_DIR) $(BUILD_FLAGS)

	@echo "=== 3. Running Game ==="
	@if [ -f "$(EXECUTABLE)" ]; then \
		$(EXECUTABLE); \
	else \
		echo "Error: Executable not found at $(EXECUTABLE)."; \
		echo "If you are on Windows, the build might have failed or placed it elsewhere."; \
	fi

# --- Utilities ---
clean:
	@echo "Removing build directory..."
	# "cmake -E remove_directory" works on Linux AND Windows (cmd/powershell)
	cmake -E remove_directory $(BUILD_DIR)

rebuild: clean game

help:
	@echo "Makefile for Penance ($(DETECTED_OS))"
	@echo "  make         : Configure, build, and run the game"
	@echo "  make clean   : Delete the build folder"
	@echo "  make rebuild : Delete build folder and start fresh"
