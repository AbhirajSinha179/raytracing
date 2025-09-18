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

# Library source files (excluding main.c)
set(LIBRARY_SOURCES
    src/lighting.c
    src/math_utils.c
    src/scene.c
    src/utils.c
)

# Create main raytracing demo
add_executable(raytracing_demo src/main.c ${LIBRARY_SOURCES})
target_link_libraries(raytracing_demo ${SDL2_LIBRARIES} m)

# Create rasterization demo
add_executable(rasterization_demo examples/rasterization_example.c ${LIBRARY_SOURCES})
target_link_libraries(rasterization_demo ${SDL2_LIBRARIES} m)

# Create performance comparison demo
add_executable(performance_comparison examples/performance_comparison.c ${LIBRARY_SOURCES})
target_link_libraries(performance_comparison ${SDL2_LIBRARIES} m)

# Set output directories
set_target_properties(raytracing_demo rasterization_demo performance_comparison PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)