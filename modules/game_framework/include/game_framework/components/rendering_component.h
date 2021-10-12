#pragma once

#include "component.h"

namespace renderer {
class graphics_api;
class texture;
} // namespace renderer

namespace gameframework {
class actor;
class rendering_component : public spectacle::component {

public:
  explicit rendering_component(spectacle::actor &owner);

  virtual void draw(renderer::graphics_api &api) noexcept = 0;
  virtual ~rendering_component() noexcept;
};
} // namespace gameframework