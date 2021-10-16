#pragma once

#include "actor.h"
#include "rendering_component.h"
#include "texture.h"

#include <memory>

namespace spectacle {
class actor;
}

namespace gameframework {
class sprite_component : public rendering_component {

private:
  std::shared_ptr<renderer::texture> m_texture;

public:
  explicit sprite_component(spectacle::actor &owner) noexcept;
  void set_texture(::std::shared_ptr<renderer::texture> new_texture);
  virtual void draw(renderer::renderer &renderer) noexcept override;

  virtual void
  initialize(const spectacle::initialization_object &object) noexcept override;

  virtual std::shared_ptr<spectacle::component>
  clone() const noexcept override {
    return nullptr;
  }
};
} // namespace gameframework