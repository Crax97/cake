#pragma once

#include "game_framework/component.h"
#include "renderer/renderer.hpp"

namespace renderer {
class graphics_api;
class texture;
} // namespace renderer

namespace gameframework {
class actor;
class rendering_component : public spectacle::component {

public:
  explicit rendering_component(spectacle::actor &owner);

  virtual void draw(renderer::renderer &renderer) noexcept = 0;
  ~rendering_component() noexcept override;
};
} // namespace gameframework