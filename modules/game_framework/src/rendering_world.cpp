#include "game_framework/rendering_world.h"
#include "game_framework/components/rendering_component.h"
#include "graphics_api.h"

#include <algorithm>

void gameframework::rendering_world::register_rendering_component(
    gameframework::rendering_component *new_component) noexcept {
  m_rendering_components.emplace_back(new_component);
}

void gameframework::rendering_world::unregister_rendering_component(
    gameframework::rendering_component *which_component) noexcept {
  auto found = std::find(m_rendering_components.begin(),
                         m_rendering_components.end(), which_component);
  if (found != m_rendering_components.end()) {
    m_rendering_components.erase(found);
  }
}

void gameframework::rendering_world::render(
    renderer::graphics_api &api, spectacle::camera &m_camera) noexcept {
  m_renderer.begin_scene(m_camera.get_view_projection_matrix());
  std::for_each(m_rendering_components.begin(), m_rendering_components.end(),
                [&](gameframework::rendering_component *which_component) {
                  which_component->draw(m_renderer);
                });
  m_renderer.flush();
}