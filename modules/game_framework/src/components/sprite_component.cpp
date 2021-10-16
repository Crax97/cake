#include "game_framework/components/sprite_component.h"
#include "game_framework/components/rendering_component.h"
#include "game_framework/loaders/texture_loader.h"
#include "glm/gtc/quaternion.hpp"
#include "initialization_object.h"

#include "actor.h"
#include "application.h"
#include "graphics_api.h"

gameframework::sprite_component::sprite_component(
    spectacle::actor &owner) noexcept
    : rendering_component(owner) {

  m_properties.emplace_back(
      std::make_shared<texture_property<sprite_component>>(
          &sprite_component::m_texture));
}

void gameframework::sprite_component::draw(
    renderer::renderer &renderer) noexcept {
  if (!m_texture)
    return;
  auto euler_rotation = glm::eulerAngles(get_owner().get_rotation());
  renderer.draw_texture(m_texture,
                        {
                            .location = get_owner().get_location(),
                            .scale = glm::vec3(100) * get_owner().get_scale(),
                            .rotation = euler_rotation.z,
                            .z_index = -1,
                        });
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