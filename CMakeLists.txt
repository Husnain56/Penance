cmake_minimum_required(VERSION 3.16)

# --- Project Configuration ---
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Essential for Nvim/Helix/LSP
project(Penance LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 1. Optimize Raylib Fetch ---
# We disable examples and games to speed up the build significantly.
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
set(PLATFORM Desktop CACHE STRING "Build for Desktop")

include(FetchContent)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        5.5 # Pinning a specific version is good practice
)
FetchContent_MakeAvailable(raylib)

# --- 2. Source Management ---
# Using GLOB is okay for small projects, but listing files is safer for large ones.
# We stick to GLOB here for "plug-and-play" ease of use.
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# --- 3. Create Executable ---
# "WIN32" creates a windowed app (no console pop-up) on Windows.
# If you want the console for debugging, remove "WIN32".
if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES})
else()
    add_executable(${PROJECT_NAME} ${SRC_FILES})
endif()

# --- 4. Link Raylib ---
# Raylib's cmake target automatically handles system dependencies (GL, pthread, winmm, etc.)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Ensure we can include headers like "player.hpp" if they are in src/
target_include_directories(${PROJECT_NAME} PRIVATE src)

# --- 5. Resource Management ---
# Copies the resources folder to the build directory automatically
if(EXISTS ${CMAKE_SOURCE_DIR}/resources)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
        COMMENT "Copying resources to binary directory..."
    )
endif()

# --- 6. Editor Support (Nvim/Helix) ---
# Copies compile_commands.json from build dir to root so LSPs find it immediately.
if(CMAKE_EXPORT_COMPILE_COMMANDS)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json
        COMMENT "Updating compile_commands.json in root for LSP support"
    )
endif()
