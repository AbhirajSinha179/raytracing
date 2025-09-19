#include "raytracing.h"
#include <stdlib.h>

#define MIN_REFLECTION_CONTRIBUTION 0.05f // no reflections below 5% contrib
#define MAX_RAY_DISTANCE 50.0f // no rays beyond this dist
#define MIN_LIGHT_CONTRIBUTION 0.01F // skip lights with minimal contribution

// Sphere intersection using ray-sphere intersection formula
bool sphere_intersect(Sphere sphere, Ray ray, HitInfo *hit_info)
{
    Vector3 oc = vector3_sub(ray.origin, sphere.center);

    float a = vector3_dot(ray.direction, ray.direction);
    float b = 2.0f * vector3_dot(oc, ray.direction);
    float c = vector3_dot(oc, oc) - sphere.radius * sphere.radius;

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
    {
        hit_info->hit = false;
        return false;
    }

    float sqrt_discriminant = sqrtf(discriminant);
    float t1 = (-b - sqrt_discriminant) / (2.0f * a);
    float t2 = (-b + sqrt_discriminant) / (2.0f * a);

    float t = (t1 > 0.001f) ? t1 : t2; // Use closest positive intersection

    if (t > 0.001f)
    {
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
                         Material material, Light lights[], int light_count)
{
    Color result = color_scale(material.color, material.ambient);

    for (int i = 0; i < light_count; i++)
    {
        Vector3 light_dir = vector3_normalize(vector3_sub(lights[i].position, point));

        // Diffuse lighting (Lambertian)
        float n_dot_l = fmaxf(0.0f, vector3_dot(normal, light_dir));
        Color diffuse = color_scale(
            color_multiply(material.color, lights[i].color),
            material.diffuse * n_dot_l * lights[i].intensity);

        // Specular lighting (Phong)
        Vector3 reflect_dir = vector3_sub(
            vector3_scale(normal, 2.0f * vector3_dot(normal, light_dir)),
            light_dir);
        float r_dot_v = fmaxf(0.0f, vector3_dot(reflect_dir, view_dir));
        float spec_factor = powf(r_dot_v, material.shininess);
        Color specular = color_scale(
            lights[i].color,
            material.specular * spec_factor * lights[i].intensity);

        result = color_add(result, color_add(diffuse, specular));
    }

    return result;
}

// Shadow calculation - test if point is in shadow from a light
bool is_in_shadow(Vector3 point, Vector3 light_pos, Scene *scene)
{
    Vector3 light_dir = vector3_sub(light_pos, point);
    float light_distance = vector3_length(light_dir);
    light_dir = vector3_normalize(light_dir);

    Ray shadow_ray;
    shadow_ray.origin = vector3_add(point, vector3_scale(light_dir, EPSILON)); // Offset to avoid self-intersection
    shadow_ray.direction = light_dir;

    for (int i = 0; i < scene->sphere_count; i++)
    {
        HitInfo hit;
        if (sphere_intersect(scene->spheres[i], shadow_ray, &hit))
        {
            if (hit.distance < light_distance)
            {
                return true; // In shadow
            }
        }
    }
    return false;
}

// Advanced ray tracing with reflections and shadows
Color trace_ray(Ray ray, Scene *scene, RenderSettings *settings, int depth)
{
    if (depth >= MAX_REFLECTIONS)
    {
        return scene->background;
    }

    HitInfo closest_hit;
    closest_hit.hit = false;
    closest_hit.distance = INFINITY;

    // Find closest intersection
    for (int i = 0; i < scene->sphere_count; i++)
    {
        HitInfo hit;
        if (sphere_intersect(scene->spheres[i], ray, &hit))
        {
            if (hit.distance < closest_hit.distance)
            {
                closest_hit = hit;
            }
        }
    }

    if (!closest_hit.hit && closest_hit.distance > MAX_RAY_DISTANCE)
    {
        return scene->background;
    }

    Vector3 view_dir = vector3_normalize(vector3_scale(ray.direction, -1.0f));
    Color result = color_create(0, 0, 0);

    // Calculate lighting from all light sources
    for (int i = 0; i < scene->light_count; i++)
    {
        bool in_shadow = settings->enable_shadows &&
                         is_in_shadow(closest_hit.point, scene->lights[i].position, scene);

        if (!in_shadow)
        {
            Vector3 light_vector = vector3_sub(scene->lights[i].position, closest_hit.point);
            float light_distance = vector3_length(light_vector);
            
            if (light_distance > MAX_RAY_DISTANCE || scene->lights[i].intensity < MIN_LIGHT_CONTRIBUTION)
            continue; // Skip lights that are too far or too weak
            
            Vector3 light_dir = vector3_normalize(light_vector);
            // Diffuse lighting
            float n_dot_l = fmaxf(0.0f, vector3_dot(closest_hit.normal, light_dir));
            Color diffuse = color_scale(
                color_multiply(closest_hit.material.color, scene->lights[i].color),
                closest_hit.material.diffuse * n_dot_l * scene->lights[i].intensity);

            // Specular lighting
            Vector3 reflect_dir = vector3_reflect(vector3_scale(light_dir, -1.0f), closest_hit.normal);
            float r_dot_v = fmaxf(0.0f, vector3_dot(reflect_dir, view_dir));
            float spec_factor = powf(r_dot_v, closest_hit.material.shininess);
            Color specular = color_scale(
                scene->lights[i].color,
                closest_hit.material.specular * spec_factor * scene->lights[i].intensity);

            result = color_add(result, color_add(diffuse, specular));
        }
    }

    // Add ambient lighting
    Color ambient = color_scale(closest_hit.material.color, closest_hit.material.ambient);
    result = color_add(result, ambient);

    // Add reflections
    if (settings->enable_reflections && closest_hit.material.specular > MIN_REFLECTION_CONTRIBUTION)
    {
        float reflection_contribution = closest_hit.material.specular * settings->reflection_strength;
        if (reflection_contribution < MIN_REFLECTION_CONTRIBUTION)
            return result; // Skip negligible reflections

        Vector3 reflect_dir = vector3_reflect(ray.direction, closest_hit.normal);
        Ray reflect_ray;
        reflect_ray.origin = vector3_add(closest_hit.point, vector3_scale(closest_hit.normal, EPSILON));
        reflect_ray.direction = reflect_dir;

        Color reflection = trace_ray(reflect_ray, scene, settings, depth + 1);
        reflection = color_scale(reflection, closest_hit.material.specular * settings->reflection_strength);
        result = color_add(result, reflection);
    }

    return result;
}

// Simplified sphere drawing for rasterization examples
void draw_sphere_simple(SDL_Renderer *renderer, int center_x, int center_y,
                        int radius, Vector3 light_pos)
{
    for (int y = -radius; y <= radius; y++)
    {
        for (int x = -radius; x <= radius; x++)
        {
            if (x * x + y * y <= radius * radius)
            {
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