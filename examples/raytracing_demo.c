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
        .reflection_strength = 0.5f};

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

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Raytracing Performance Comparison",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Create scene with multiple spheres and lights
    Scene *scene = scene_create();

    Material red_material = {
        .color = color_create(1.0f, 0.2f, 0.2f),
        .ambient = 0.1f,
        .diffuse = 0.7f,
        .specular = 0.5f,
        .shininess = 32.0f};

    Material green_material = {
        .color = color_create(0.2f, 1.0f, 0.2f),
        .ambient = 0.1f,
        .diffuse = 0.7f,
        .specular = 0.3f,
        .shininess = 16.0f};

    Material blue_material = {
        .color = color_create(0.2f, 0.2f, 1.0f),
        .ambient = 0.1f,
        .diffuse = 0.7f,
        .specular = 0.4f,
        .shininess = 8.0f};

    Material metallic_material = {
        .color = color_create(0.8f, 0.8f, 0.8f),
        .ambient = 0.1f,
        .diffuse = 0.5f,
        .specular = 0.9f,
        .shininess = 64.0f};
    
    scene_add_sphere(scene, vector3_create(-1.5f, 0.0f, -5.0f), 1.0f, red_material);
    scene_add_sphere(scene, vector3_create(1.5f, 0.0f, -4.0f), 1.0f, green_material);
    scene_add_sphere(scene, vector3_create(0.0f, -1.5f, -6.0f), 1.0f, blue_material);
    scene_add_sphere(scene, vector3_create(0.0f, 1.5f, -7.0f), 1.0f, metallic_material);
    scene_add_light(scene, vector3_create(5.0f, 5.0f, 0.0f), color_create(1.0f, 1.0f, 1.0f), 1.0f);
    scene_add_light(scene, vector3_create(-3.0f, 5.0f, 2.0f), color_create(0.5f, 0.5f, 1.0f), 0.7f);
    scene_add_light(scene, vector3_create(0.0f, -5.0f, 5.0f), color_create(1.0f, 0.8f, 0.6f), 0.5f);
    scene->background = color_create(0.1f, 0.1f, 0.2f);
    Camera camera = camera_create(
        vector3_create(0.0f, 0.0f, 0.0f),  // position
        vector3_create(0.0f, 0.0f, -1.0f), // target
        vector3_create(0.0f, 1.0f, 0.0f),  // up
        45.0f                              // field of view
    );

    BenchmarkResult results[4];
    benchmark_simple_rasterization(renderer, &results[0]);
    benchmark_basic_raytracing(renderer, scene, &results[1]);
    benchmark_advanced_raytracing(renderer, scene, &camera, &results[2]);
    benchmark_anti_aliased_raytracing(renderer, scene, &camera, &results[3]);

    print_benchmark_results(results, 4);

    // Wait for user to close window
    SDL_Event event;
    bool running = true;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }
        SDL_Delay(100);
    }

    scene_destroy(scene);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}