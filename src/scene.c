#include "raytracing.h"
#include <stdlib.h>

// Scene management
Scene* scene_create(void) {
    Scene* scene = (Scene*)malloc(sizeof(Scene));
    if (!scene) return NULL;
    
    scene->sphere_count = 0;
    scene->light_count = 0;
    scene->background = color_create(0.1f, 0.1f, 0.2f); // Dark blue background
    
    return scene;
}

void scene_destroy(Scene* scene) {
    if (scene) {
        free(scene);
    }
}

void scene_add_sphere(Scene* scene, Vector3 center, float radius, Material material) {
    if (scene && scene->sphere_count < MAX_SPHERES) {
        scene->spheres[scene->sphere_count].center = center;
        scene->spheres[scene->sphere_count].radius = radius;
        scene->spheres[scene->sphere_count].material = material;
        scene->sphere_count++;
    }
}

void scene_add_light(Scene* scene, Vector3 position, Color color, float intensity) {
    if (scene && scene->light_count < 5) {
        scene->lights[scene->light_count].position = position;
        scene->lights[scene->light_count].color = color;
        scene->lights[scene->light_count].intensity = intensity;
        scene->light_count++;
    }
}

// Ray creation for camera
Ray create_camera_ray(int x, int y, Vector3 camera_pos) {
    Ray ray;
    ray.origin = camera_pos;
    
    // Convert screen coordinates to normalized device coordinates
    float ndc_x = (2.0f * x / WINDOW_WIDTH) - 1.0f;
    float ndc_y = 1.0f - (2.0f * y / WINDOW_HEIGHT);
    
    // Simple perspective projection
    ray.direction = vector3_normalize(vector3_create(ndc_x, ndc_y, -1.0f));
    
    return ray;
}

// Main rendering function with proper raytracing
void render_scene(SDL_Renderer* renderer, Scene* scene, Vector3 camera_pos) {
    for (int y = 0; y < WINDOW_HEIGHT; y++) {
        for (int x = 0; x < WINDOW_WIDTH; x++) {
            Ray ray = create_camera_ray(x, y, camera_pos);
            
            HitInfo closest_hit;
            closest_hit.hit = false;
            closest_hit.distance = INFINITY;
            
            // Test intersection with all spheres
            for (int i = 0; i < scene->sphere_count; i++) {
                HitInfo hit;
                if (sphere_intersect(scene->spheres[i], ray, &hit)) {
                    if (hit.distance < closest_hit.distance) {
                        closest_hit = hit;
                    }
                }
            }
            
            Color pixel_color;
            if (closest_hit.hit) {
                Vector3 view_dir = vector3_normalize(vector3_sub(camera_pos, closest_hit.point));
                pixel_color = calculate_lighting(closest_hit.point, closest_hit.normal, 
                                               view_dir, closest_hit.material, 
                                               scene->lights, scene->light_count);
            } else {
                pixel_color = scene->background;
            }
            
            // Convert color to RGB and draw pixel
            Uint8 r = (Uint8)(fmaxf(0.0f, fminf(1.0f, pixel_color.r)) * 255);
            Uint8 g = (Uint8)(fmaxf(0.0f, fminf(1.0f, pixel_color.g)) * 255);
            Uint8 b = (Uint8)(fmaxf(0.0f, fminf(1.0f, pixel_color.b)) * 255);
            
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}