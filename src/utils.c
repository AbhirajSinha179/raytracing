#include "raytracing.h"

// Graphics initialization and cleanup
int init_graphics(SDL_Window **window, SDL_Renderer **renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return -1;
    }

    *window = SDL_CreateWindow("Advanced Raytracing Showcase",
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!*window)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer)
    {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return -1;
    }

    return 0;
}

void cleanup_graphics(SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void handle_events(SDL_Event *event, bool *running, Vector3 *light_pos, RenderSettings *settings, Camera *camera)
{
    while (SDL_PollEvent(event))
    {
        switch (event->type)
        {
        case SDL_QUIT:
            *running = false;
            break;
        case SDL_MOUSEMOTION:
            // Update light position based on mouse
            light_pos->x = (float)event->motion.x / WINDOW_WIDTH * 10.0f - 5.0f;
            light_pos->y = (float)(WINDOW_HEIGHT - event->motion.y) / WINDOW_HEIGHT * 10.0f - 5.0f;
            break;
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym)
            {
            case SDLK_ESCAPE:
                *running = false;
                break;
            case SDLK_SPACE:
                // Reset light position
                *light_pos = vector3_create(2.0f, 2.0f, 5.0f);
                break;
            case SDLK_1:
                // Toggle shadows
                settings->enable_shadows = !settings->enable_shadows;
                printf("Shadows: %s\n", settings->enable_shadows ? "ON" : "OFF");
                break;
            case SDLK_2:
                // Toggle reflections
                settings->enable_reflections = !settings->enable_reflections;
                printf("Reflections: %s\n", settings->enable_reflections ? "ON" : "OFF");
                break;
            case SDLK_3:
                // Toggle anti-aliasing
                settings->enable_anti_aliasing = !settings->enable_anti_aliasing;
                printf("Anti-aliasing: %s\n", settings->enable_anti_aliasing ? "ON" : "OFF");
                break;
            case SDLK_w:
                // Move camera forward
                camera->position = vector3_add(camera->position, vector3_create(0, 0, -0.5f));
                break;
            case SDLK_s:
                // Move camera backward
                camera->position = vector3_add(camera->position, vector3_create(0, 0, 0.5f));
                break;
            case SDLK_a:
                // Move camera left
                camera->position = vector3_add(camera->position, vector3_create(-0.5f, 0, 0));
                break;
            case SDLK_d:
                // Move camera right
                camera->position = vector3_add(camera->position, vector3_create(0.5f, 0, 0));
                break;
            }
            break;
        }
    }
}