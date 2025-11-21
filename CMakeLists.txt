cmake_minimum_required(VERSION 3.16)

# --- Project Configuration ---
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Essential for Nvim/Helix LSP
project(Penance LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 1. Optimize Raylib Fetch ---
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
set(PLATFORM       Desktop CACHE STRING "Build for Desktop")

include(FetchContent)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        5.5
)
FetchContent_MakeAvailable(raylib)

# --- 2. Source Management ---
# CONFIGURE_DEPENDS: If you add a new .cpp file, CMake detects it automatically on next build.
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")

# --- 3. Create Executable ---
# NOTE: We deliberately do NOT use the "WIN32" flag here.
# Removing "WIN32" ensures Visual Studio looks for "int main()" instead of "WinMain()".
# This fixes the "Unresolved External Symbol" error on MSVC.
add_executable(${PROJECT_NAME} ${SRC_FILES})

# --- 4. Link Raylib ---
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_include_directories(${PROJECT_NAME} PRIVATE src)

# --- 5. Visual Studio (MSVC) Specific Fixes ---
if(MSVC)
    # 1. Force Console Subsystem: Ensures std::cout works and prevents linker errors.
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")

    # 2. Explicitly link Windows System Libraries
    # Visual Studio sometimes fails to inherit these from Raylib automatically.
    target_link_libraries(${PROJECT_NAME} PRIVATE
        winmm
        kernel32
        opengl32
        gdi32
        user32
        shell32
    )
endif()

# --- 6. Resource Management ---
# Copies resources to the build folder (works for ./build/ and VS out/ directories)
if(EXISTS ${CMAKE_SOURCE_DIR}/resources)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
        COMMENT "Copying resources to binary directory..."
    )
endif()

# --- 7. Editor Support (Nvim/Helix) ---
# Copies compile_commands.json to root for LSP support
if(CMAKE_EXPORT_COMPILE_COMMANDS)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json
        COMMENT "Updating compile_commands.json in root for LSP support"
    )
endif()
