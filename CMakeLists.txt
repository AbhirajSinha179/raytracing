cmake_minimum_required(VERSION 3.10)
project(RaytracingShowcase VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find SDL2
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(include)

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O2")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -DDEBUG")

# Source files
file(GLOB_RECURSE SOURCES "src/*.c")

# Create executable
add_executable(raytracing_demo ${SOURCES})

# Link libraries
target_link_libraries(raytracing_demo ${SDL2_LIBRARIES} m)

# Set output directory
set_target_properties(raytracing_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)