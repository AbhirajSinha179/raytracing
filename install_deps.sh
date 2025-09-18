# Installation script for Ubuntu/Debian systems
#!/bin/bash

echo "=== Raytracing Graphics Showcase Setup ==="
echo "Installing dependencies..."

# Update package lists
sudo apt-get update

# Install required packages
sudo apt-get install -y \
    cmake \
    build-essential \
    libsdl2-dev \
    pkg-config \
    git

echo "Dependencies installed successfully!"
echo ""
echo "To build the project:"
echo "  mkdir build && cd build"
echo "  cmake .. && make"
echo ""
echo "To run demos:"
echo "  ./bin/raytracing_demo"
echo "  ./bin/performance_comparison"
echo "  ./bin/rasterization_demo"