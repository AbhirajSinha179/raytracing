# Advanced Raytracing Graphics Showcase

A comprehensive demonstration of computer graphics programming techniques, featuring both rasterization and ray tracing implementations with advanced lighting models, shadows, reflections, and performance analysis.

## 🎯 Project Highlights

### **Advanced Graphics Features**

-   **Physically-Based Ray Tracing** with proper mathematical intersection calculations
-   **Phong Lighting Model** with ambient, diffuse, and specular components
-   **Real-Time Shadows** using ray-based occlusion testing
-   **Recursive Reflections** with depth-limited ray bouncing
-   **Multi-Sample Anti-Aliasing (MSAA)** for high-quality rendering
-   **Interactive Camera System** with WASD movement controls

### **Technical Excellence**

-   **Modular C Architecture** with clean separation of concerns
-   **Professional Build System** using CMake with cross-platform support
-   **Performance Benchmarking Suite** with quantitative analysis
-   **Comprehensive Documentation** of graphics concepts and implementation
-   **Multiple Demo Programs** showcasing different techniques

## 🚀 Quick Start

```bash
# Clone and build
git clone <your-repo>
cd raytracing
mkdir build && cd build
cmake .. && make

# Run the main demo
./bin/raytracing_demo

# Run performance benchmarks
./bin/performance_comparison
```

## 📊 Performance Results

Based on 800x600 resolution benchmarks:

-   **Rasterization**: 42.1 FPS - Fast but basic
-   **Basic Raytracing**: 9.9 FPS - Realistic lighting
-   **Advanced Features**: 5.8 FPS - Shadows + reflections
-   **Anti-Aliased**: 1.6 FPS - Highest quality

## 🏗️ Project Structure

```
├── include/raytracing.h     # Complete API definitions
├── src/                     # Core graphics library
│   ├── math_utils.c        # 3D vector mathematics
│   ├── lighting.c          # Ray tracing and lighting
│   ├── scene.c             # Scene management
│   ├── utils.c             # SDL2 utilities
│   └── main.c              # Main raytracing demo
├── examples/               # Additional demonstrations
│   ├── rasterization_example.c
│   └── performance_comparison.c
├── docs/                   # Technical documentation
└── CMakeLists.txt          # Professional build system
```

## 🎮 Interactive Controls

-   **Mouse**: Dynamic light positioning
-   **WASD**: Camera movement (forward/back/left/right)
-   **Number Keys**: Toggle rendering features
    -   `1`: Shadows on/off
    -   `2`: Reflections on/off
    -   `3`: Anti-aliasing on/off
-   **Space**: Reset light position
-   **ESC**: Exit


---

_Built with C, SDL2, and CMake | Real-time interactive graphics programming_
