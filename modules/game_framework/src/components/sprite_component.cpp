#include "game_framework/components/sprite_component.h"
#include "game_framework/components/rendering_component.h"
#include "game_framework/initialization_object.h"
#include "game_framework/loaders/texture_loader.h"
#include "glm/gtc/quaternion.hpp"

#include "game_framework/actor.h"
#include "graphics_api.h"
#include "sprite_component.gen.h"


gameframework::sprite_component::sprite_component(spectacle::actor &owner) noexcept
    : rendering_component(owner) {
}


void gameframework::sprite_component::draw(
    renderer::renderer &renderer) noexcept {
  if (!m_texture)
    return;
  glm::vec2 m_sprite_size = {m_texture.get_texture()->get_width(), m_texture.get_texture()->get_height()};
  auto euler_rotation = glm::eulerAngles(get_owner().get_rotation());
  renderer.draw_texture(m_texture.get_texture(),
                        {
                            .location = get_owner().get_location(),
                            .scale = m_sprite_size * m_sprite_scale *
                                     glm::vec2(get_owner().get_scale()),
                            .rotation = euler_rotation.z,
                            .z_index = -1,
                        });
}

void gameframework::sprite_component::initialize(
    const spectacle::initialization_object &object) noexcept {

  auto texture_path = object.get<std::string>("texture_path");
  if (texture_path.empty())
    return;
  auto texture_maybe =
      gameframework::texture_loader::load_texture(texture_path);
  if (texture_maybe.has_value()) {
    m_texture = texture_maybe.value();
  }
}