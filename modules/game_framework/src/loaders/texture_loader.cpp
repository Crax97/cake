#include "game_framework/loaders/texture_loader.h"
#include "application.h"
#include "graphics_api.h"
#include "stb/stb_image.h"
#include "texture.h"

std::optional<std::shared_ptr<renderer::texture>>
gameframework::texture_loader::load_texture(
    const std::filesystem::path &path) noexcept {

  int width, height, channels, wanted_channels = 4;
  renderer::texture_format fmt = renderer::texture_format::rgba;
  unsigned char *image_data =
      stbi_load(path.c_str(), &width, &height, &channels, wanted_channels);
  if (!image_data)
    return nullptr;

  if (channels == 1) {
    fmt = renderer::texture_format::red;
  } else if (channels == 3) {
    fmt = renderer::texture_format::rgb;
  }
  auto &api = app_framework::application::the().get_api();
  auto texture = api.create_texture(image_data, width, height, fmt);
  stbi_image_free(image_data);
  return texture;
}