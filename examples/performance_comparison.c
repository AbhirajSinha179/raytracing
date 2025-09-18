#include "../include/raytracing.h"
#include <time.h>

// Performance benchmark structure
typedef struct
{
    const char *name;
    int total_pixels;
    float render_time;
    float fps;
} BenchmarkResult;

void benchmark_simple_rasterization(SDL_Renderer *renderer, BenchmarkResult *result)
{
    clock_t start = clock();
    Vector3 light_pos = vector3_create(400, 200, 100);

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
    SDL_RenderClear(renderer);

    // Draw multiple spheres
    draw_sphere_simple(renderer, 200, 150, 80, light_pos);
    draw_sphere_simple(renderer, 400, 200, 60, light_pos);
    draw_sphere_simple(renderer, 600, 250, 100, light_pos);
    draw_sphere_simple(renderer, 300, 350, 70, light_pos);

    SDL_RenderPresent(renderer);

    clock_t end = clock();
    result->render_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    result->total_pixels = WINDOW_WIDTH * WINDOW_HEIGHT;
    result->fps = 1.0f / result->render_time;
    result->name = "Simple Rasterization";
}

void benchmark_basic_raytracing(SDL_Renderer *renderer, Scene *scene, BenchmarkResult *result)
{
    clock_t start = clock();
    Vector3 camera_pos = vector3_create(0, 0, 0);

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    render_scene(renderer, scene, camera_pos);
    SDL_RenderPresent(renderer);

    clock_t end = clock();
    result->render_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    result->total_pixels = WINDOW_WIDTH * WINDOW_HEIGHT;
    result->fps = 1.0f / result->render_time;
    result->name = "Basic Raytracing";
}

void benchmark_advanced_raytracing(SDL_Renderer *renderer, Scene *scene, Camera *camera, BenchmarkResult *result)
{
    clock_t start = clock();

    RenderSettings settings = {
        .enable_shadows = true,
        .enable_reflections = true,
        .enable_anti_aliasing = false,
        .samples_per_pixel = 1,
        .reflection_strength = 0.3f};

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    render_scene_advanced(renderer, scene, camera, &settings);
    SDL_RenderPresent(renderer);

    clock_t end = clock();
    result->render_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    result->total_pixels = WINDOW_WIDTH * WINDOW_HEIGHT;
    result->fps = 1.0f / result->render_time;
    result->name = "Advanced Raytracing (Shadows + Reflections)";
}

void benchmark_anti_aliased_raytracing(SDL_Renderer *renderer, Scene *scene, Camera *camera, BenchmarkResult *result)
{
    clock_t start = clock();

    RenderSettings settings = {
        .enable_shadows = true,
        .enable_reflections = true,
        .enable_anti_aliasing = true,
        .samples_per_pixel = 4,
        .reflection_strength = 0.3f};

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    render_scene_advanced(renderer, scene, camera, &settings);
    SDL_RenderPresent(renderer);

    clock_t end = clock();
    result->render_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    result->total_pixels = WINDOW_WIDTH * WINDOW_HEIGHT * settings.samples_per_pixel;
    result->fps = 1.0f / result->render_time;
    result->name = "Anti-Aliased Raytracing (4x MSAA)";
}

void print_benchmark_results(BenchmarkResult *results, int count)
{
    printf("\n==== GRAPHICS RENDERING PERFORMANCE COMPARISON ====\n");
    printf("Resolution: %dx%d pixels\n", WINDOW_WIDTH, WINDOW_HEIGHT);
    printf("%-40s | %10s | %8s | %12s\n", "Technique", "Time (s)", "FPS", "Pixels/sec");
    printf("%-40s-|-%10s-|-%8s-|-%12s\n", "----------------------------------------",
           "----------", "--------", "------------");

    for (int i = 0; i < count; i++)
    {
        float pixels_per_sec = results[i].total_pixels / results[i].render_time;
        printf("%-40s | %10.3f | %8.1f | %12.0f\n",
               results[i].name,
               results[i].render_time,
               results[i].fps,
               pixels_per_sec);
    }
    printf("\nKey Observations:\n");
    printf("- Rasterization is fastest but least realistic\n");
    printf("- Basic raytracing adds realistic lighting\n");
    printf("- Advanced features (shadows, reflections) increase quality but reduce performance\n");
    printf("- Anti-aliasing significantly improves quality at high performance cost\n");
}

int main()
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (init_graphics(&window, &renderer) != 0)
    {
        return 1;
    }

    // Create test scene
    Scene *scene = scene_create();
    if (!scene)
    {
        fprintf(stderr, "Failed to create scene\n");
        cleanup_graphics(window, renderer);
        return 1;
    }

    // Set up materials
    Material red_material = {color_create(0.8f, 0.2f, 0.2f), 0.1f, 0.8f, 0.9f, 64.0f};
    Material blue_material = {color_create(0.2f, 0.2f, 0.8f), 0.1f, 0.7f, 0.8f, 128.0f};
    Material green_material = {color_create(0.2f, 0.8f, 0.2f), 0.1f, 0.6f, 0.3f, 16.0f};
    Material metallic_material = {color_create(0.9f, 0.9f, 0.9f), 0.05f, 0.3f, 0.95f, 256.0f};

    // Add spheres
    scene_add_sphere(scene, vector3_create(0.0f, 0.0f, -5.0f), 1.0f, red_material);
    scene_add_sphere(scene, vector3_create(-2.5f, 0.0f, -4.0f), 0.8f, blue_material);
    scene_add_sphere(scene, vector3_create(2.5f, -1.0f, -6.0f), 1.2f, green_material);
    scene_add_sphere(scene, vector3_create(0.0f, -2.0f, -4.5f), 0.6f, metallic_material);

    // Add lights
    scene_add_light(scene, vector3_create(3.0f, 3.0f, 2.0f), color_create(1.0f, 1.0f, 1.0f), 1.0f);
    scene_add_light(scene, vector3_create(-2.0f, 1.0f, 1.0f), color_create(0.3f, 0.3f, 0.8f), 0.5f);

    // Create camera
    Camera camera = camera_create(
        vector3_create(0.0f, 0.0f, 0.0f),
        vector3_create(0.0f, 0.0f, -1.0f),
        vector3_create(0.0f, 1.0f, 0.0f),
        45.0f);

    printf("Running Graphics Performance Benchmarks...\n");
    printf("This will render the same scene using different techniques.\n");
    printf("Press any key to continue between tests.\n\n");

    BenchmarkResult results[4];
    SDL_Event event;
    bool continue_benchmarks = true;

    // Benchmark 1: Simple Rasterization
    printf("1. Benchmarking Simple Rasterization...\n");
    benchmark_simple_rasterization(renderer, &results[0]);
    printf("   Completed in %.3f seconds (%.1f FPS)\n", results[0].render_time, results[0].fps);

    // Wait for user input
    printf("   Press any key to continue...\n");
    while (continue_benchmarks)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)
            {
                if (event.type == SDL_QUIT)
                {
                    continue_benchmarks = false;
                    goto cleanup;
                }
                goto next1;
            }
        }
        SDL_Delay(100);
    }
next1:

    // Benchmark 2: Basic Raytracing
    printf("\n2. Benchmarking Basic Raytracing...\n");
    benchmark_basic_raytracing(renderer, scene, &results[1]);
    printf("   Completed in %.3f seconds (%.1f FPS)\n", results[1].render_time, results[1].fps);

    printf("   Press any key to continue...\n");
    while (continue_benchmarks)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)
            {
                if (event.type == SDL_QUIT)
                {
                    continue_benchmarks = false;
                    goto cleanup;
                }
                goto next2;
            }
        }
        SDL_Delay(100);
    }
next2:

    // Benchmark 3: Advanced Raytracing
    printf("\n3. Benchmarking Advanced Raytracing (Shadows + Reflections)...\n");
    benchmark_advanced_raytracing(renderer, scene, &camera, &results[2]);
    printf("   Completed in %.3f seconds (%.1f FPS)\n", results[2].render_time, results[2].fps);

    printf("   Press any key to continue...\n");
    while (continue_benchmarks)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)
            {
                if (event.type == SDL_QUIT)
                {
                    continue_benchmarks = false;
                    goto cleanup;
                }
                goto next3;
            }
        }
        SDL_Delay(100);
    }
next3:

    // Benchmark 4: Anti-Aliased Raytracing
    printf("\n4. Benchmarking Anti-Aliased Raytracing (4x MSAA)...\n");
    printf("   This may take a while...\n");
    benchmark_anti_aliased_raytracing(renderer, scene, &camera, &results[3]);
    printf("   Completed in %.3f seconds (%.1f FPS)\n", results[3].render_time, results[3].fps);

    // Print comprehensive results
    print_benchmark_results(results, 4);

    printf("\nPress any key to exit...\n");
    while (continue_benchmarks)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)
            {
                goto cleanup;
            }
        }
        SDL_Delay(100);
    }

cleanup:
    scene_destroy(scene);
    cleanup_graphics(window, renderer);
    return 0;
}