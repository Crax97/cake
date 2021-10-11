#include "game_framework/components/sprite_component.h"
#include "game_framework/loaders/texture_loader.h"
#include "initialization_object.h"

#include "application.h"
#include "graphics_api.h"

void gameframework::sprite_component::draw(
    renderer::graphics_api &api) noexcept {
  if (!m_texture)
    return;
}

void gameframework::sprite_component::initialize(
    const spectacle::initialization_object &object) noexcept {

  auto texture_path = object.get<std::string>("texture_path");
  if (texture_path.size() == 0)
    return;
  auto texture_maybe =
      gameframework::texture_loader::load_texture(texture_path);
  if (texture_maybe.has_value()) {
    m_texture = texture_maybe.value();
  }
}