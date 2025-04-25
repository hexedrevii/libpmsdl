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

    /// @brief register a tile for drawing.
    /// @returns true or false depending on success status.
    bool add_tile(int id, const std::filesystem::path& path);

    /// @brief register a tile for drawing, relative to the binary path.
    /// @returns true or false depending on success status.
    bool add_tile_relative(int id, const std::filesystem::path& relative);

    /// @brief draw all layers, starting from the offset.
    void draw();

    /// @brief draw a single layer, starting from the offset.
    void draw_layer(int layer);
  private:
    std::map<int, std::unique_ptr<SDL_Texture>> m_tiles;
    nlohmann::json m_data;
  };
}
