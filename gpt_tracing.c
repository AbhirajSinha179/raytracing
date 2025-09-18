#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Function to compute lighting intensity based on the angle between the normal and light direction
Uint8 compute_lighting(int x, int y, int light_x, int light_y, int r)
{
  // Compute the normal vector at this point
  float nx = (float)(x - WINDOW_WIDTH / 2) / r;
  float ny = (float)(y - WINDOW_HEIGHT / 2) / r;
  float nl = sqrtf(nx * nx + ny * ny);
  if (nl > 0)
  {
    nx /= nl;
    ny /= nl;
  }

  // Compute light direction vector
  float lx = (float)(light_x - x);
  float ly = (float)(light_y - y);
  float ll = sqrtf(lx * lx + ly * ly);
  if (ll > 0)
  {
    lx /= ll;
    ly /= ll;
  }

  // Compute dot product for lighting intensity
  float intensity = fmaxf(0.0f, nx * lx + ny * ly);

  return (Uint8)(intensity * 255);
}

void draw_filled_circle(SDL_Renderer *renderer, int xc, int yc, int r, int light_x, int light_y)
{
  for (int y = -r; y <= r; y++)
  {
    for (int x = -r; x <= r; x++)
    {
      if (x * x + y * y <= r * r)
      {
        Uint8 shade = compute_lighting(xc + x, yc + y, light_x, light_y, r);
        SDL_SetRenderDrawColor(renderer, shade, shade, shade, 255);
        SDL_RenderDrawPoint(renderer, xc + x, yc + y);
      }
    }
  }
}

int main()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("Filled Circle Rasterization with Lighting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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

  bool running = true;
  SDL_Event event;
  int light_x = WINDOW_WIDTH / 2, light_y = WINDOW_HEIGHT / 2;

  while (running)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        running = false;
      }
      else if (event.type == SDL_MOUSEMOTION)
      {
        light_x = event.motion.x;
        light_y = event.motion.y;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw_filled_circle(renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 100, light_x, light_y);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
