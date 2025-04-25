#pragma once

#include <SDL3/SDL.h>

namespace MapExample
{
  class Renderer
  {
  public:
    Renderer(SDL_Renderer* renderer, SDL_Window* window)
    {
      this->m_render = renderer;
      this->m_window = window;
    }

    /**
     * @brief Creates a new renderer texture with specified dimensions.
     * @param width Texture width in pixels.
     * @param height Texture height in pixels.
     * @return true if creation succeeded, false on error.
     */
    bool create(int width, int height);

    /**
     * @brief Binds the renderer as the current drawing target.
     * @note All subsequent draw operations will affect this texture.
     */
    void attach();

    /**
     * @brief Unbinds the renderer and flushes all pending draw commands.
     * @details Equivalent to detach() + immediate GPU submission.
     */
    void detach_and_draw();

    /**
     * @brief Destroys the renderer and releases SDL resources.
     * @warning The object becomes unusable after this call.
     */
    void destroy()
    {
      SDL_DestroyTexture(this->m_renderer);
    }

  private:
    SDL_Texture *m_renderer = nullptr;
    SDL_Renderer *m_render;
    SDL_Window *m_window;
    int m_width = 0;
    int m_height = 0;
  };
}
