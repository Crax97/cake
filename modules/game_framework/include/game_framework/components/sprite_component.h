#pragma once

#include "game_framework/actor.h"
#include "rendering_component.h"
#include "texture.h"

#include <list>
#include <memory>

namespace spectacle {
class actor;
}

namespace gameframework {
class REFLECT sprite_component : public rendering_component {
    GENERATE_REFLECT_BODY(sprite_component)
private:
  std::shared_ptr<renderer::texture> REFLECT m_texture {nullptr};
  glm::vec2 REFLECT m_sprite_scale{1};

public:
  explicit sprite_component(spectacle::actor &owner) noexcept;
  void set_texture(::std::shared_ptr<renderer::texture> new_texture);
  virtual void draw(renderer::renderer &renderer) noexcept override;

  void
  initialize(const spectacle::initialization_object &object) noexcept override;

  virtual std::shared_ptr<spectacle::component>
  clone() const noexcept override {
    return nullptr;
  }
};
} // namespace gameframework