cmake_minimum_required(VERSION 3.16)
project(Penance LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Fetch latest raylib from GitHub ---
include(FetchContent)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
)

FetchContent_MakeAvailable(raylib)

# --- Add your game sources ---
file(GLOB SRC_FILES src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# --- Link against raylib ---
target_link_libraries(${PROJECT_NAME} raylib)

# --- Platform-specific dependencies ---
if (WIN32)
    target_link_libraries(${PROJECT_NAME} winmm gdi32)
elseif (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} pthread m dl GL)
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME}
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo")
endif()