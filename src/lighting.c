#include "raytracing.h"
#include <stdlib.h>

// Sphere intersection using ray-sphere intersection formula
bool sphere_intersect(Sphere sphere, Ray ray, HitInfo* hit_info) {
    Vector3 oc = vector3_sub(ray.origin, sphere.center);
    
    float a = vector3_dot(ray.direction, ray.direction);
    float b = 2.0f * vector3_dot(oc, ray.direction);
    float c = vector3_dot(oc, oc) - sphere.radius * sphere.radius;
    
    float discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0) {
        hit_info->hit = false;
        return false;
    }
    
    float sqrt_discriminant = sqrtf(discriminant);
    float t1 = (-b - sqrt_discriminant) / (2.0f * a);
    float t2 = (-b + sqrt_discriminant) / (2.0f * a);
    
    float t = (t1 > 0.001f) ? t1 : t2; // Use closest positive intersection
    
    if (t > 0.001f) {
        hit_info->hit = true;
        hit_info->distance = t;
        hit_info->point = vector3_add(ray.origin, vector3_scale(ray.direction, t));
        hit_info->normal = vector3_normalize(vector3_sub(hit_info->point, sphere.center));
        hit_info->material = sphere.material;
        return true;
    }
    
    hit_info->hit = false;
    return false;
}

// Improved lighting calculation with Phong shading model
Color calculate_lighting(Vector3 point, Vector3 normal, Vector3 view_dir, 
                        Material material, Light lights[], int light_count) {
    Color result = color_scale(material.color, material.ambient);
    
    for (int i = 0; i < light_count; i++) {
        Vector3 light_dir = vector3_normalize(vector3_sub(lights[i].position, point));
        
        // Diffuse lighting (Lambertian)
        float n_dot_l = fmaxf(0.0f, vector3_dot(normal, light_dir));
        Color diffuse = color_scale(
            color_multiply(material.color, lights[i].color), 
            material.diffuse * n_dot_l * lights[i].intensity
        );
        
        // Specular lighting (Phong)
        Vector3 reflect_dir = vector3_sub(
            vector3_scale(normal, 2.0f * vector3_dot(normal, light_dir)), 
            light_dir
        );
        float r_dot_v = fmaxf(0.0f, vector3_dot(reflect_dir, view_dir));
        float spec_factor = powf(r_dot_v, material.shininess);
        Color specular = color_scale(
            lights[i].color, 
            material.specular * spec_factor * lights[i].intensity
        );
        
        result = color_add(result, color_add(diffuse, specular));
    }
    
    return result;
}

// Simplified sphere drawing for rasterization examples
void draw_sphere_simple(SDL_Renderer* renderer, int center_x, int center_y, 
                       int radius, Vector3 light_pos) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                // Calculate surface normal
                Vector3 surface_point = vector3_create(center_x + x, center_y + y, 0);
                Vector3 center = vector3_create(center_x, center_y, 0);
                Vector3 normal = vector3_normalize(vector3_sub(surface_point, center));
                
                // Calculate light direction
                Vector3 light_dir = vector3_normalize(vector3_sub(light_pos, surface_point));
                
                // Simple diffuse lighting
                float intensity = fmaxf(0.1f, vector3_dot(normal, light_dir));
                Uint8 shade = (Uint8)(intensity * 255);
                
                SDL_SetRenderDrawColor(renderer, shade, shade, shade, 255);
                SDL_RenderDrawPoint(renderer, center_x + x, center_y + y);
            }
        }
    }
}