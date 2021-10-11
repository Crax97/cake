#pragma once

#include "rendering_component.h"
#include "texture.h"

#include <memory>

namespace gameframework {
class sprite_component : public rendering_component {

private:
  std::shared_ptr<renderer::texture> m_texture;

public:
  sprite_component() noexcept;
  void set_texture(::std::shared_ptr<renderer::texture> new_texture);
  virtual void draw(renderer::graphics_api &api) noexcept override;

  virtual void
  initialize(const spectacle::initialization_object &object) noexcept override;
};
} // namespace gameframework