#include "raytracing.h"

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (init_graphics(&window, &renderer) != 0)
    {
        return 1;
    }

    // Create scene
    Scene *scene = scene_create();
    if (!scene)
    {
        fprintf(stderr, "Failed to create scene\n");
        cleanup_graphics(window, renderer);
        return 1;
    }

    // Set up materials with varying properties
    Material red_material = {
        color_create(0.8f, 0.2f, 0.2f), // Bright red
        0.1f,                           // ambient
        0.8f,                           // diffuse
        0.9f,                           // high specular for reflections
        64.0f                           // high shininess
    };

    Material blue_material = {
        color_create(0.2f, 0.2f, 0.8f), // Bright blue
        0.1f, 0.7f, 0.8f, 128.0f        // Very reflective
    };

    Material green_material = {
        color_create(0.2f, 0.8f, 0.2f), // Bright green
        0.1f, 0.6f, 0.3f, 16.0f         // Matte finish
    };

    Material metallic_material = {
        color_create(0.9f, 0.9f, 0.9f), // Metallic silver
        0.05f, 0.3f, 0.95f, 256.0f      // Highly reflective
    };

    // Add spheres to scene with better positioning
    scene_add_sphere(scene, vector3_create(0.0f, 0.0f, -5.0f), 1.0f, red_material);
    scene_add_sphere(scene, vector3_create(-2.5f, 0.0f, -4.0f), 0.8f, blue_material);
    scene_add_sphere(scene, vector3_create(2.5f, -1.0f, -6.0f), 1.2f, green_material);
    scene_add_sphere(scene, vector3_create(0.0f, -2.0f, -4.5f), 0.6f, metallic_material);

    // Add multiple lights for better scene illumination
    Vector3 main_light = vector3_create(3.0f, 3.0f, 2.0f);
    scene_add_light(scene, main_light, color_create(1.0f, 1.0f, 1.0f), 1.0f);
    scene_add_light(scene, vector3_create(-2.0f, 1.0f, 1.0f), color_create(0.3f, 0.3f, 0.8f), 0.5f);

    // Create camera
    Camera camera = camera_create(
        vector3_create(0.0f, 0.0f, 0.0f),  // position
        vector3_create(0.0f, 0.0f, -1.0f), // target
        vector3_create(0.0f, 1.0f, 0.0f),  // up
        45.0f                              // field of view
    );

    // Render settings
    RenderSettings settings = {
        .enable_shadows = true,
        .enable_reflections = true,
        .enable_anti_aliasing = false, // Start with AA off for performance
        .samples_per_pixel = 4,
        .reflection_strength = 0.3f};

    bool running = true;
    SDL_Event event;

    printf("Advanced Raytracing Showcase Controls:\n");
    printf("- Mouse: Control main light position\n");
    printf("- WASD: Move camera (W=forward, S=back, A=left, D=right)\n");
    printf("- 1: Toggle shadows\n");
    printf("- 2: Toggle reflections\n");
    printf("- 3: Toggle anti-aliasing (performance impact)\n");
    printf("- SPACE: Reset light position\n");
    printf("- ESC: Exit\n");
    printf("Rendering with shadows and reflections enabled...\n");

    while (running)
    {
        handle_events(&event, &running, &main_light, &settings, &camera);

        // Update main light in scene
        scene->lights[0].position = main_light;

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render using advanced raytracing
        render_scene_advanced(renderer, scene, &camera, &settings);

        SDL_RenderPresent(renderer);

        // Small delay to prevent excessive CPU usage
        SDL_Delay(16);
    }

    scene_destroy(scene);
    cleanup_graphics(window, renderer);
    return 0;
}