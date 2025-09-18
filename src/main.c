#include "raytracing.h"

// Graphics initialization and cleanup
int init_graphics(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return -1;
    }

    *window = SDL_CreateWindow("Advanced Raytracing Showcase",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!*window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return -1;
    }

    return 0;
}

void cleanup_graphics(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void handle_events(SDL_Event* event, bool* running, Vector3* light_pos) {
    while (SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_QUIT:
                *running = false;
                break;
            case SDL_MOUSEMOTION:
                // Update light position based on mouse
                light_pos->x = (float)event->motion.x / WINDOW_WIDTH * 10.0f - 5.0f;
                light_pos->y = (float)(WINDOW_HEIGHT - event->motion.y) / WINDOW_HEIGHT * 10.0f - 5.0f;
                break;
            case SDL_KEYDOWN:
                switch (event->key.keysym.sym) {
                    case SDLK_ESCAPE:
                        *running = false;
                        break;
                    case SDLK_SPACE:
                        // Reset light position
                        *light_pos = vector3_create(2.0f, 2.0f, 5.0f);
                        break;
                }
                break;
        }
    }
}

int main(int argc __attribute__((unused)), char* argv[] __attribute__((unused))) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    
    if (init_graphics(&window, &renderer) != 0) {
        return 1;
    }

    // Create scene
    Scene* scene = scene_create();
    if (!scene) {
        fprintf(stderr, "Failed to create scene\n");
        cleanup_graphics(window, renderer);
        return 1;
    }

    // Set up materials
    Material red_material = {
        color_create(0.8f, 0.2f, 0.2f),  // color
        0.1f,   // ambient
        0.8f,   // diffuse
        0.5f,   // specular
        32.0f   // shininess
    };

    Material blue_material = {
        color_create(0.2f, 0.2f, 0.8f),
        0.1f, 0.8f, 0.7f, 64.0f
    };

    Material green_material = {
        color_create(0.2f, 0.8f, 0.2f),
        0.1f, 0.6f, 0.3f, 16.0f
    };

    // Add spheres to scene
    scene_add_sphere(scene, vector3_create(0.0f, 0.0f, -3.0f), 1.0f, red_material);
    scene_add_sphere(scene, vector3_create(-2.0f, 0.0f, -4.0f), 0.8f, blue_material);
    scene_add_sphere(scene, vector3_create(2.0f, -1.0f, -5.0f), 1.2f, green_material);

    // Add lights
    Vector3 light_pos = vector3_create(2.0f, 2.0f, 5.0f);
    scene_add_light(scene, light_pos, color_create(1.0f, 1.0f, 1.0f), 1.0f);

    // Camera position
    Vector3 camera_pos = vector3_create(0.0f, 0.0f, 0.0f);

    bool running = true;
    SDL_Event event;

    printf("Raytracing Showcase Controls:\n");
    printf("- Move mouse to control light position\n");
    printf("- Press SPACE to reset light position\n");
    printf("- Press ESC to exit\n");
    printf("Rendering...\n");

    while (running) {
        handle_events(&event, &running, &light_pos);
        
        // Update light in scene
        scene->lights[0].position = light_pos;

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render the scene using proper raytracing
        render_scene(renderer, scene, camera_pos);

        SDL_RenderPresent(renderer);
        
        // Small delay to prevent excessive CPU usage
        SDL_Delay(16);
    }

    scene_destroy(scene);
    cleanup_graphics(window, renderer);
    return 0;
}