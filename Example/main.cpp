#include "renderer.h"
#include "p8m/map.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <iostream>

int main()
{
  // SDL INITIALISATION
  // ---------------------------
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("ERROR: SDL: Cannot initialise SDL: %s.", SDL_GetError());
    exit(1);
  }

  SDL_Window* window = SDL_CreateWindow("Hello Map!", 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  if (!window)
  {
    SDL_Log("ERROR: SDL: Cannot initialise SDL Window: %s.", SDL_GetError());
    SDL_Quit();
    
    exit(1);
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
  if (!renderer)
  {
    SDL_Log("ERROR: SDL: Cannot create renderer: %s.", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();

    exit(1);
  }

  MapExample::Renderer small_renderer = MapExample::Renderer(renderer, window);
  if (!small_renderer.create(128, 128))
  {
    SDL_Log("ERROR: RENDERER: Cannot create small renderer.");
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    SDL_Quit();
  }
  
  // --------------------------

  P8M::Map map = P8M::Map::from_relative("Content/map.p8m", renderer);

  std::cout << "Grid Size: " << map.grid_size.x << ", " << map.grid_size.y << std::endl;
  std::cout << "Tile Size: " << map.tile_size.x << ", " << map.tile_size.y << std::endl;
  std::cout << "Layers: " << map.layers.size() << std::endl;

  map.add_tile_relative(1, "Content/wall.png", SDL_SCALEMODE_NEAREST);
  map.add_tile_relative(2, "Content/box.png", SDL_SCALEMODE_NEAREST);
  map.add_tile_relative(3, "Content/goal.png", SDL_SCALEMODE_NEAREST);
  map.add_tile_relative(4, "Content/player.png", SDL_SCALEMODE_NEAREST);
    
  bool running = true;
  while (running)
  {
    // SDL Event
    // --------------------------------
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_EVENT_QUIT)
      {
        running = false;
      }
    }
    // --------------------------------
    
    // Clear window outside small renderer
    // --------------------------------------------------
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // --------------------------------------------------

    small_renderer.attach();
    
    // Clear window inside small renderer
    // --------------------------------------------------
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    // --------------------------------------------------
    
    // TODO: Draw map
    
    small_renderer.detach_and_draw();

    // Move to the next frame
    SDL_RenderPresent(renderer);
  }

  SDL_Log("Quitting game.");

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  small_renderer.destroy();

  SDL_Quit();
}
