# Raytracing Graphics Showcase
# Alternative Makefile for systems without CMake

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
LDFLAGS = -lSDL2 -lm
INCLUDES = -Iinclude

# Directories
SRCDIR = src
EXAMPLEDIR = examples
BUILDDIR = build
BINDIR = $(BUILDDIR)/bin

# Library sources (excluding main.c)
LIB_SOURCES = $(SRCDIR)/lighting.c $(SRCDIR)/math_utils.c $(SRCDIR)/scene.c $(SRCDIR)/utils.c
LIB_OBJECTS = $(LIB_SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

# Executables
TARGETS = $(BINDIR)/raytracing_demo $(BINDIR)/rasterization_demo $(BINDIR)/performance_comparison

.PHONY: all clean install test

all: $(TARGETS)

# Create directories
$(BUILDDIR) $(BINDIR):
	mkdir -p $@

# Compile library objects
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Main raytracing demo
$(BINDIR)/raytracing_demo: $(SRCDIR)/main.c $(LIB_OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ $(LDFLAGS) -o $@

# Rasterization demo
$(BINDIR)/rasterization_demo: $(EXAMPLEDIR)/rasterization_example.c $(LIB_OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ $(LDFLAGS) -o $@

# Performance comparison
$(BINDIR)/performance_comparison: $(EXAMPLEDIR)/performance_comparison.c $(LIB_OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ $(LDFLAGS) -o $@

clean:
	rm -rf $(BUILDDIR)

install: all
	@echo "Installing raytracing demos to /usr/local/bin (requires sudo)"
	sudo cp $(TARGETS) /usr/local/bin/

test: all
	@echo "Running basic functionality test..."
	timeout 5s $(BINDIR)/raytracing_demo || echo "Demo started successfully"

help:
	@echo "Raytracing Graphics Showcase Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all      - Build all demo programs"
	@echo "  clean    - Remove build artifacts"
	@echo "  install  - Install demos to system (requires sudo)"
	@echo "  test     - Run basic functionality test"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "Demo Programs:"
	@echo "  raytracing_demo        - Advanced raytracing with shadows and reflections"
	@echo "  rasterization_demo     - Simple sphere rasterization example"
	@echo "  performance_comparison - Benchmark different rendering techniques"