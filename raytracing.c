#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

// Function to draw a circle using Bresenham's Algorithm
void drawCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius)
{
  int x = radius, y = 0;
  int p = 1 - radius; // Decision parameter

  while (x >= y)
  {
    // Plot eight symmetric points
    SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
    SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
    SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
    SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
    SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
    SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
    SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
    SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
 
    y++;

    if (p <= 0)
    {
      p += 2 * y + 1;
    }
    else
    {
      x--;
      p += 2 * (y - x) + 1;
    }
  }
}

int compute_intensity(int x, int y, int radius)
{
  float nx = (float)(x - 400) / radius;
  float ny = (float)(y - 300) / radius;
  
  float lx = -1.0f;
  float ly = 0.0f;
  // float ly = (float)(300 - y);

  float intensity = fmaxf(0.0f, nx * lx + ny * ly);

  return (int)(intensity * 255);
}

void drawFilledCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius)
{
  for (int y = -radius; y <= radius; y++)
  {
    for (int x = -radius; x <= radius; x++)
    {
      if (x * x + y * y <= radius * radius)
      {
        int shade = compute_intensity(centerX + x, centerY + y, radius);
        SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        SDL_SetRenderDrawColor(renderer, shade, shade, shade, 255);
      }
    }
  }
}

int main(int argc, char *argv[])
{
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  // Create SDL window
  SDL_Window *window = SDL_CreateWindow("SDL2 Draggable Circle",
                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        800, 600, SDL_WINDOW_SHOWN);
  if (!window)
  {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  // Create SDL renderer
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer)
  {
    printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  // Circle properties
  int circleX = 400, circleY = 300, radius = 100;
  int dragging = 0; // Flag to check if the circle is being dragged
  int running = 1;  // Flag to keep the main loop running

  SDL_Event event;
  while (running)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        running = 0; // Exit loop if window is closed
      }
      else if (event.type == SDL_MOUSEBUTTONDOWN)
      {
        // Check if click is inside the circle
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        int dx = mouseX - circleX;
        int dy = mouseY - circleY;

        if (dx * dx + dy * dy <= radius * radius)
        {
          dragging = 1; // Enable dragging if clicked inside
        }
      }
      else if (event.type == SDL_MOUSEBUTTONUP)
      {
        dragging = 0; // Stop dragging on mouse release
      }
      else if (event.type == SDL_MOUSEMOTION && dragging)
      {
        // Move the circle with the mouse
        SDL_GetMouseState(&circleX, &circleY);
      }
    }

    // Clear screen (black background)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Set render color to white (circle)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    drawFilledCircle(renderer, circleX, circleY, radius);

    // Present the new frame
    SDL_RenderPresent(renderer);

    // Small delay to control frame rate
    SDL_Delay(16);
  }

  // Cleanup
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
