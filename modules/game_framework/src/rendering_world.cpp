#include "game_framework/rendering_world.h"
#include "game_framework/components/rendering_component.h"
#include "graphics_api.h"

#include <algorithm>

void gameframework::rendering_world::register_rendering_component(
    std::shared_ptr<gameframework::rendering_component>
        new_component) noexcept {
  m_rendering_components.emplace_back(std::move(new_component));
}

void gameframework::rendering_world::unregister_rendering_component(
    std::shared_ptr<gameframework::rendering_component>
        which_component) noexcept {
  auto found = std::find(m_rendering_components.begin(),
                         m_rendering_components.end(), which_component);
  if (found != m_rendering_components.end()) {
    m_rendering_components.erase(found);
  }
}

void gameframework::rendering_world::render(
    renderer::graphics_api &api) noexcept {
  std::for_each(m_rendering_components.begin(), m_rendering_components.end(),
                [&api](std::shared_ptr<gameframework::rendering_component>
                           &which_component) { which_component->draw(api); });
}