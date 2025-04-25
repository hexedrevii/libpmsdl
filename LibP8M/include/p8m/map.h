#pragma once

#include <filesystem>
#include <map>

#include <SDL3/SDL.h>
#include <nlohmann/json.hpp>
#include <vector>

namespace P8M
{
  class Map
  {
  public:
    // Fields
    
    SDL_Point offset = {0, 0};
    
    SDL_Point tile_size;
    SDL_Point grid_size;
    std::vector<std::vector<std::vector<int>>> layers;


    // Initialisers
    
    /// @brief loads a map from an absolute path.
    Map(const std::filesystem::path& path);

    /// @brief loads a .p8m file, relative to the binary path.
    static Map from_relative(const std::filesystem::path& relative);


    // Library functions

    /// @brief draw all layers, starting from the offset.
    void draw();

    /// @brief draw a single layer, starting from the offset.
    void draw_layer(int layer);
  private:
    std::map<int, SDL_Texture*> m_tiles;
    nlohmann::json m_data;
  };
}
