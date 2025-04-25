#include "p8m/map.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_render.h>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <format>
#include <fstream>

#include <SDL3_image/SDL_image.h>

P8M::Map::Map(const std::filesystem::path& path, SDL_Renderer* renderer)
{
  if (!std::filesystem::exists(path))
    throw std::runtime_error(std::format("Path {} does not exist!", path.string()));

  std::ifstream mdata(path);
  if (!mdata.is_open())
    throw std::runtime_error(std::format("Could not open file at path {}.", path.string()));

  this->m_data = nlohmann::json::parse(mdata);

  this->layers = this->m_data["layers"];
  
  this->tile_size = SDL_Point { this->m_data["tile_x"], this->m_data["tile_y"] };
  this->grid_size = SDL_Point { this->m_data["grid_x"], this->m_data["grid_y"] };
  
  mdata.close();

  this->m_renderer = renderer;
}

P8M::Map P8M::Map::from_relative(const std::filesystem::path &relative, SDL_Renderer* renderer)
{
  const char* base = SDL_GetBasePath();
  if (!base)
    throw std::runtime_error(std::format("Cannot load base executable path: {}", SDL_GetError()));

  std::filesystem::path full = base / relative;
  return P8M::Map(full, renderer);
}

bool P8M::Map::add_tile(int id, const std::filesystem::path& path, SDL_ScaleMode mode)
{
  if (!this->m_renderer)
  {
    SDL_Log("ERROR: Renderer is set to null.");
    return false;
  }
  
  if (!std::filesystem::exists(path))
  {
    SDL_Log("ERROR: Provided path doesn't exist!");
    return false;
  } 

  std::string legacy = path.string();
  std::unique_ptr<SDL_Texture, Internal::SDLEraser> tex(IMG_LoadTexture(this->m_renderer, legacy.c_str()));
  if (!tex)
  {
    SDL_Log("ERROR: Failed to load texture: %s!", SDL_GetError());
    return false;
  }

  SDL_SetTextureScaleMode(tex.get(), mode);
  this->m_tiles[id] = std::move(tex);
  
  return true;
}

bool P8M::Map::add_tile_relative(int id, const std::filesystem::path& relative, SDL_ScaleMode mode)
{
  const char* base = SDL_GetBasePath();
  if (!base)
  {
    SDL_Log("ERROR: Unable to load base executable path: %s.", SDL_GetError());
    return false;
  }

  return this->add_tile(id, base / relative, mode);
}
