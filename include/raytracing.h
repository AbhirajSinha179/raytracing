#ifndef RAYTRACING_H
#define RAYTRACING_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// Constants
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MAX_SPHERES 10

// Vector3 structure for 3D coordinates
typedef struct {
    float x, y, z;
} Vector3;

// Color structure
typedef struct {
    float r, g, b;
} Color;

// Material properties
typedef struct {
    Color color;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
} Material;

// Sphere structure
typedef struct {
    Vector3 center;
    float radius;
    Material material;
} Sphere;

// Light structure
typedef struct {
    Vector3 position;
    Color color;
    float intensity;
} Light;

// Scene structure
typedef struct {
    Sphere spheres[MAX_SPHERES];
    int sphere_count;
    Light lights[5];
    int light_count;
    Color background;
} Scene;

// Ray structure
typedef struct {
    Vector3 origin;
    Vector3 direction;
} Ray;

// Hit information
typedef struct {
    bool hit;
    float distance;
    Vector3 point;
    Vector3 normal;
    Material material;
} HitInfo;

// Function declarations
Vector3 vector3_create(float x, float y, float z);
Vector3 vector3_add(Vector3 a, Vector3 b);
Vector3 vector3_sub(Vector3 a, Vector3 b);
Vector3 vector3_scale(Vector3 v, float s);
float vector3_dot(Vector3 a, Vector3 b);
Vector3 vector3_normalize(Vector3 v);
float vector3_length(Vector3 v);

Color color_create(float r, float g, float b);
Color color_add(Color a, Color b);
Color color_scale(Color c, float s);
Color color_multiply(Color a, Color b);

// Lighting calculations
Color calculate_lighting(Vector3 point, Vector3 normal, Vector3 view_dir, 
                        Material material, Light lights[], int light_count);
Uint8 lighting_to_grayscale(Color color);

// Sphere operations
bool sphere_intersect(Sphere sphere, Ray ray, HitInfo* hit_info);
void draw_sphere_simple(SDL_Renderer* renderer, int center_x, int center_y, 
                       int radius, Vector3 light_pos);

// Scene management
Scene* scene_create(void);
void scene_destroy(Scene* scene);
void scene_add_sphere(Scene* scene, Vector3 center, float radius, Material material);
void scene_add_light(Scene* scene, Vector3 position, Color color, float intensity);

// Rendering
void render_scene(SDL_Renderer* renderer, Scene* scene, Vector3 camera_pos);
Ray create_camera_ray(int x, int y, Vector3 camera_pos);

// Application management
int init_graphics(SDL_Window** window, SDL_Renderer** renderer);
void cleanup_graphics(SDL_Window* window, SDL_Renderer* renderer);
void handle_events(SDL_Event* event, bool* running, Vector3* light_pos);

#endif // RAYTRACING_H