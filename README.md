# Raytracing Showcase

A comprehensive graphics programming demonstration featuring raytracing, rasterization, and advanced lighting techniques implemented in C with SDL2.

## Features

### Core Graphics Techniques
- **Ray-Sphere Intersection**: Mathematical ray tracing with proper intersection calculations
- **Phong Lighting Model**: Ambient, diffuse, and specular lighting components
- **Multiple Light Sources**: Support for up to 5 dynamic light sources
- **Material Properties**: Configurable ambient, diffuse, specular, and shininess values
- **Interactive Controls**: Mouse-controlled lighting and keyboard shortcuts

### Technical Implementation
- **Modular Architecture**: Clean separation of concerns across multiple modules
- **Vector Mathematics**: Complete 3D vector operations library
- **Scene Management**: Flexible scene graph with multiple objects and lights
- **Real-time Rendering**: Interactive frame-rate rendering with SDL2

## Building the Project

### Prerequisites
- CMake 3.10 or higher
- SDL2 development libraries
- GCC or compatible C compiler

### Ubuntu/Debian Installation
```bash
sudo apt-get update
sudo apt-get install cmake libsdl2-dev build-essential
```

### Build Instructions
```bash
mkdir build
cd build
cmake ..
make
```

### Running the Demo
```bash
./bin/raytracing_demo
```

## Controls
- **Mouse Movement**: Control light position in real-time
- **SPACE**: Reset light to default position
- **ESC**: Exit application

## Project Structure
```
├── CMakeLists.txt          # Build configuration
├── include/
│   └── raytracing.h        # Main header with all declarations
├── src/
│   ├── main.c              # Application entry point and scene setup
│   ├── math_utils.c        # Vector and color mathematics
│   ├── lighting.c          # Lighting calculations and sphere intersection
│   └── scene.c             # Scene management and rendering pipeline
├── docs/                   # Documentation
└── examples/               # Example implementations
```

## Graphics Concepts Demonstrated

### 1. Ray Tracing
- Ray-sphere intersection using quadratic formula
- Proper handling of multiple intersections
- View ray generation from screen coordinates

### 2. Lighting Models
- **Lambertian (Diffuse)**: Surface brightness based on light angle
- **Phong Specular**: Reflective highlights with configurable shininess
- **Ambient**: Base illumination to prevent pure black shadows

### 3. Color Theory
- RGB color mixing and blending
- Color space conversions
- Gamma-correct color calculations

### 4. 3D Mathematics
- Vector operations (dot product, normalization, scaling)
- 3D transformations and projections
- Normal vector calculations for lighting

## Performance Considerations
- Pixel-by-pixel raytracing (suitable for learning, not production)
- Real-time interactivity with optimized intersection tests
- Memory-efficient scene representation

## Future Enhancements
- Reflection and refraction
- Texture mapping
- Shadow calculations
- Anti-aliasing techniques
- GPU acceleration with compute shaders

## Learning Resources
This project demonstrates fundamental computer graphics concepts suitable for:
- Computer graphics coursework
- Self-directed graphics programming learning
- Portfolio demonstration of mathematical and programming skills
- Foundation for more advanced rendering techniques