#pragma once

#include "game_framework/actor.h"
#include "rendering_component.h"
#include "texture.h"

#include "game_framework/properties/property.h"

#include <list>
#include <memory>

namespace spectacle {
class actor;
}

namespace gameframework {
class sprite_component : public rendering_component {

private:
  std::shared_ptr<renderer::texture> m_texture;
  glm::vec2 m_sprite_scale{1};

public:
  explicit sprite_component(spectacle::actor &owner) noexcept;
  void set_texture(::std::shared_ptr<renderer::texture> new_texture);
  virtual void draw(renderer::renderer &renderer) noexcept override;

  virtual void
  initialize(const spectacle::initialization_object &object) noexcept override;

  virtual const char *get_component_name() const noexcept override {
    return "Sprite Component";
  }

  virtual std::shared_ptr<spectacle::component>
  clone() const noexcept override {
    return nullptr;
  }
};
} // namespace gameframework