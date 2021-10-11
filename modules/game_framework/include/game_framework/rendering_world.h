#pragma once

#include "graphics_api.h"
#include <memory>
#include <vector>

namespace renderer {
class graphics_api;
}

namespace gameframework {

class rendering_component;

class rendering_world {
private:
  std::vector<std::shared_ptr<rendering_component>> m_rendering_components;

public:
  explicit rendering_world(renderer::graphics_api &api) noexcept {}

  void register_rendering_component(
      std::shared_ptr<rendering_component> new_component) noexcept;

  void unregister_rendering_component(
      std::shared_ptr<rendering_component> which_component) noexcept;

  void render(renderer::graphics_api &api) noexcept;
};
} // namespace gameframework