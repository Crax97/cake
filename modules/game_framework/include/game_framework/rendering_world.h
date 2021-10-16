#pragma once

#include "camera.h"
#include "graphics_api.h"
#include <memory>
#include <vector>

#include "renderer/renderer.hpp"

namespace renderer {
class graphics_api;
}

namespace gameframework {

class rendering_component;

class rendering_world {
private:
  std::vector<rendering_component *> m_rendering_components;
  renderer::renderer m_renderer;

public:
  explicit rendering_world(renderer::graphics_api &api) noexcept
      : m_renderer(api) {}

  void register_rendering_component(
      gameframework::rendering_component *new_component) noexcept;

  void unregister_rendering_component(
      gameframework::rendering_component *which_component) noexcept;

  void render(renderer::graphics_api &api,
              spectacle::camera &m_camera) noexcept;
};
} // namespace gameframework