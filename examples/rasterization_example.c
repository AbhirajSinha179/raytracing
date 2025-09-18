#include "../include/raytracing.h"

// Simple event handling for rasterization demo
void handle_raster_events(SDL_Event *event, bool *running, Vector3 *light_pos)
{
    while (SDL_PollEvent(event))
    {
        switch (event->type)
        {
        case SDL_QUIT:
            *running = false;
            break;
        case SDL_MOUSEMOTION:
            light_pos->x = (float)event->motion.x;
            light_pos->y = (float)event->motion.y;
            light_pos->z = 100.0f;
            break;
        case SDL_KEYDOWN:
            if (event->key.keysym.sym == SDLK_ESCAPE)
            {
                *running = false;
            }
            break;
        }
    }
}

// Example demonstrating simple rasterization (from your original code)
void rasterization_example(SDL_Renderer *renderer, Vector3 light_pos)
{
    // Draw multiple spheres with simple lighting
    draw_sphere_simple(renderer, 200, 200, 80, light_pos);
    draw_sphere_simple(renderer, 400, 300, 60, light_pos);
    draw_sphere_simple(renderer, 600, 250, 100, light_pos);
}

int main()
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (init_graphics(&window, &renderer) != 0)
    {
        return 1;
    }

    bool running = true;
    SDL_Event event;
    Vector3 light_pos = vector3_create(400, 200, 100);

    printf("Rasterization Example - Simple sphere rendering with mouse-controlled lighting\n");
    printf("- Move mouse to control light position\n");
    printf("- Press ESC to exit\n");

    while (running)
    {
        handle_raster_events(&event, &running, &light_pos);

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
        SDL_RenderClear(renderer);

        // Render using rasterization
        rasterization_example(renderer, light_pos);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    cleanup_graphics(window, renderer);
    return 0;
}