#include "raytracing.h"
#include <stdlib.h>

// Scene management
Scene *scene_create(void)
{
    Scene *scene = (Scene *)malloc(sizeof(Scene));
    if (!scene)
        return NULL;

    scene->sphere_count = 0;
    scene->light_count = 0;
    scene->background = color_create(0.1f, 0.1f, 0.2f); // Dark blue background

    return scene;
}

void scene_destroy(Scene *scene)
{
    if (scene)
    {
        free(scene);
    }
}

void scene_add_sphere(Scene *scene, Vector3 center, float radius, Material material)
{
    if (scene && scene->sphere_count < MAX_SPHERES)
    {
        scene->spheres[scene->sphere_count].center = center;
        scene->spheres[scene->sphere_count].radius = radius;
        scene->spheres[scene->sphere_count].material = material;
        scene->sphere_count++;
    }
}

void scene_add_light(Scene *scene, Vector3 position, Color color, float intensity)
{
    if (scene && scene->light_count < 5)
    {
        scene->lights[scene->light_count].position = position;
        scene->lights[scene->light_count].color = color;
        scene->lights[scene->light_count].intensity = intensity;
        scene->light_count++;
    }
}

// Ray creation for camera
Ray create_camera_ray(int x, int y, Vector3 camera_pos)
{
    Ray ray;
    ray.origin = camera_pos;

    // Convert screen coordinates to normalized device coordinates
    float ndc_x = (2.0f * x / WINDOW_WIDTH) - 1.0f;
    float ndc_y = 1.0f - (2.0f * y / WINDOW_HEIGHT);

    // Simple perspective projection
    ray.direction = vector3_normalize(vector3_create(ndc_x, ndc_y, -1.0f));

    return ray;
}

// Camera functions
Camera camera_create(Vector3 position, Vector3 target, Vector3 up, float fov)
{
    Camera camera;
    camera.position = position;
    camera.target = target;
    camera.up = vector3_normalize(up);
    camera.fov = fov;
    camera.aspect_ratio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    return camera;
}

Ray camera_get_ray(Camera camera, float u, float v)
{
    // Calculate camera coordinate system
    Vector3 w = vector3_normalize(vector3_sub(camera.position, camera.target));
    Vector3 u_vec = vector3_normalize(vector3_cross(camera.up, w));
    Vector3 v_vec = vector3_cross(w, u_vec);

    // Calculate viewport dimensions
    float viewport_height = 2.0f * tanf(camera.fov * M_PI / 360.0f);
    float viewport_width = camera.aspect_ratio * viewport_height;

    Vector3 horizontal = vector3_scale(u_vec, viewport_width);
    Vector3 vertical = vector3_scale(v_vec, viewport_height);
    Vector3 lower_left = vector3_sub(vector3_sub(vector3_sub(camera.position, vector3_scale(horizontal, 0.5f)), vector3_scale(vertical, 0.5f)), w);

    Ray ray;
    ray.origin = camera.position;
    ray.direction = vector3_normalize(vector3_sub(vector3_add(vector3_add(lower_left, vector3_scale(horizontal, u)), vector3_scale(vertical, v)), camera.position));

    return ray;
}

// Main rendering function with proper raytracing
void render_scene(SDL_Renderer *renderer, Scene *scene, Vector3 camera_pos)
{
    for (int y = 0; y < WINDOW_HEIGHT; y++)
    {
        for (int x = 0; x < WINDOW_WIDTH; x++)
        {
            Ray ray = create_camera_ray(x, y, camera_pos);

            HitInfo closest_hit;
            closest_hit.hit = false;
            closest_hit.distance = INFINITY;

            // Test intersection with all spheres
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

            Color pixel_color;
            if (closest_hit.hit)
            {
                Vector3 view_dir = vector3_normalize(vector3_sub(camera_pos, closest_hit.point));
                pixel_color = calculate_lighting(closest_hit.point, closest_hit.normal,
                                                 view_dir, closest_hit.material,
                                                 scene->lights, scene->light_count);
            }
            else
            {
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

// Advanced rendering with all features
void render_scene_advanced(SDL_Renderer *renderer, Scene *scene, Camera *camera, RenderSettings *settings)
{
    static int frame_count = 0;
    static Uint32 start_time = 0;

    if (frame_count == 0)
    {
        start_time = SDL_GetTicks();
    }

    for (int y = 0; y < WINDOW_HEIGHT; y++)
    {
        for (int x = 0; x < WINDOW_WIDTH; x++)
        {
            Color pixel_color = color_create(0, 0, 0);

            if (settings->enable_anti_aliasing)
            {
                // Multi-sampling for anti-aliasing
                for (int sample = 0; sample < settings->samples_per_pixel; sample++)
                {
                    float u = ((float)x + ((float)rand() / RAND_MAX)) / (float)WINDOW_WIDTH;
                    float v = ((float)(WINDOW_HEIGHT - y) + ((float)rand() / RAND_MAX)) / (float)WINDOW_HEIGHT;

                    Ray ray = camera_get_ray(*camera, u, v);
                    Color sample_color = trace_ray(ray, scene, settings, 0);
                    pixel_color = color_add(pixel_color, sample_color);
                }
                pixel_color = color_scale(pixel_color, 1.0f / settings->samples_per_pixel);
            }
            else
            {
                float u = (float)x / (float)WINDOW_WIDTH;
                float v = (float)(WINDOW_HEIGHT - y) / (float)WINDOW_HEIGHT;

                Ray ray = camera_get_ray(*camera, u, v);
                pixel_color = trace_ray(ray, scene, settings, 0);
            }

            // Convert color to RGB and draw pixel
            Uint8 r = (Uint8)(fmaxf(0.0f, fminf(1.0f, pixel_color.r)) * 255);
            Uint8 g = (Uint8)(fmaxf(0.0f, fminf(1.0f, pixel_color.g)) * 255);
            Uint8 b = (Uint8)(fmaxf(0.0f, fminf(1.0f, pixel_color.b)) * 255);

            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    frame_count++;
    if (frame_count % 60 == 0)
    {
        float elapsed = (SDL_GetTicks() - start_time) / 1000.0f;
        print_performance_stats(frame_count, elapsed);
    }
}

// Performance monitoring
void print_performance_stats(int frame_count, float total_time)
{
    float fps = frame_count / total_time;
    printf("Performance: %d frames in %.2fs (%.1f FPS)\n", frame_count, total_time, fps);
}