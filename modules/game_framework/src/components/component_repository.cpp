#include "game_framework/components/component_repository.h"
#include "component.h"

#include <unordered_map>

gameframework::component_repository &
gameframework::component_repository::the() noexcept {
  static component_repository repo = component_repository();
  return repo;
}

void gameframework::component_repository::register_new_component(
    const std::string &component_name,
    component_constructor_fn &&creator) noexcept {
  m_components.emplace(component_name, creator);
}
std::optional<std::shared_ptr<spectacle::component>>
gameframework::component_repository::construct_component(
    const std::string &component_name, spectacle::actor &owner) noexcept {
  auto component_constructor_maybe = m_components.find(component_name);
  if (component_constructor_maybe == m_components.end()) {
    return {};
  }
  auto &constructor = component_constructor_maybe->second;
  return constructor(owner);
}
std::vector<std::string>
gameframework::component_repository::get_component_names() noexcept {
  std::vector<std::string> components;
  for (const auto &pair : m_components) {
    components.push_back(pair.first);
  }
  return components;
}