cmake_minimum_required(VERSION 3.16)

# --- Project Configuration ---
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
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
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")

# --- 3. Create Executable ---
add_executable(${PROJECT_NAME} ${SRC_FILES})

# --- 4. Link Raylib ---
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_include_directories(${PROJECT_NAME} PRIVATE src)

# --- 5. Visual Studio (MSVC) Specific Fixes ---
if(MSVC)
    # Force Console so we can see cout/printf
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")

    # Link required Windows libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE
        winmm kernel32 opengl32 gdi32 user32 shell32
    )
endif()

# --- 6. Resource Management ---
# We check if directory exists at configure time to prevent build errors
if(EXISTS "${CMAKE_SOURCE_DIR}/resources")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/resources"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
        COMMENT "Copying resources to binary directory..."
    )
endif()

# --- 7. Editor Support (Nvim/Helix) ---
# FIX: We only copy compile_commands.json if NOT using MSVC.
# Visual Studio (MSVC) does not generate this file, so trying to copy it causes a crash.
if(CMAKE_EXPORT_COMPILE_COMMANDS AND NOT MSVC)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json
        COMMENT "Updating compile_commands.json in root for LSP support"
    )
endif()
