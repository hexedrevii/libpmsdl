#include "p8m/map.h"
#include <SDL3/SDL_filesystem.h>
#include <filesystem>
#include <stdexcept>
#include <format>
#include <fstream>

P8M::Map::Map(const std::filesystem::path& path)
{
  if (!std::filesystem::exists(path))
    throw std::runtime_error(std::format("Path {} does not exist!", path.string()));

  std::ifstream mdata(path);
  if (!mdata.is_open())
    throw std::runtime_error(std::format("Could not open file at path {}.", path.string()));

  this->layers = this->m_data["layers"];
  
  this->tile_size = SDL_Point { this->m_data["tile_x"], this->m_data["tile_y"] };
  this->grid_size = SDL_Point { this->m_data["grid_x"], this->m_data["grid_y"] };
  
  mdata.close();
}

P8M::Map P8M::Map::from_relative(const std::filesystem::path &relative)
{
  const char* base = SDL_GetBasePath();
  if (!base)
    throw std::runtime_error(std::format("Cannot load base executable path: {}", SDL_GetError()));

  std::filesystem::path full = base / relative;
  return P8M::Map(full);
}

