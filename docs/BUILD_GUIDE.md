# Build and Run Instructions

## Quick Start

```bash
# Using CMake (recommended)
mkdir build && cd build
cmake ..
make
./bin/raytracing_demo

# Using Makefile (alternative)
make -f Makefile help
make -f Makefile all
```

## Demo Programs

### 1. Main Raytracing Demo (`./bin/raytracing_demo`)

**Features**: Full raytracing with shadows, reflections, and interactive controls
**Controls**:

-   Mouse: Control light position
-   WASD: Move camera
-   1/2/3: Toggle shadows/reflections/anti-aliasing
-   ESC: Exit

### 2. Rasterization Demo (`./bin/rasterization_demo`)

**Features**: Traditional sphere rasterization with simple lighting
**Purpose**: Compare rasterization vs raytracing approaches
**Controls**: Mouse controls lighting, ESC to exit

### 3. Performance Comparison (`./bin/performance_comparison`)

**Features**: Benchmarks all rendering techniques
**Purpose**: Quantify performance differences between approaches
**Output**: Detailed performance metrics and analysis

## Technical Achievements

### Graphics Programming Skills Demonstrated

✅ **3D Mathematics**: Vector operations, matrix transformations, geometric calculations
✅ **Ray Tracing**: Intersection algorithms, recursive ray bouncing
✅ **Lighting Models**: Phong shading, multiple light sources, shadow casting
✅ **Optimization**: Performance benchmarking, algorithmic complexity analysis
✅ **Software Engineering**: Modular design, build systems, documentation

### Advanced Graphics Features

✅ **Shadows**: Ray-based occlusion testing
✅ **Reflections**: Recursive ray tracing with depth limiting
✅ **Anti-Aliasing**: Multi-sample anti-aliasing (MSAA)
✅ **Materials**: Ambient, diffuse, specular, and shininess properties
✅ **Camera System**: Perspective projection with interactive controls

## Code Quality Indicators

-   **Modular Architecture**: Clean separation between math, rendering, and application logic
-   **Professional Build System**: CMake with proper dependency management
-   **Comprehensive Documentation**: Technical concepts and implementation details
-   **Performance Analysis**: Benchmarking tools with quantitative metrics
-   **Error Handling**: Proper resource management and error checking
