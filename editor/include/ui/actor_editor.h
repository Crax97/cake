#pragma once

#include <memory>
#include <string_view>

namespace spectacle {
class stage;
class actor;
} // namespace spectacle

class editor_application;

namespace editor {
class actor_editor {
private:
  std::weak_ptr<spectacle::actor> m_selected_actor;
    bool wants_to_add_component = false;
public:
  void draw_editor() noexcept;
  void
  show_actor_fields(const std::shared_ptr<spectacle::actor>& the_actor) noexcept;
  static void
  show_actor_components(std::shared_ptr<spectacle::actor> the_actor) noexcept;
  void select_actor(std::shared_ptr<spectacle::actor> new_actor);
  std::shared_ptr<spectacle::actor> get_selected_actor() noexcept {
    if (m_selected_actor.expired()) {
      return nullptr;
    }

    return m_selected_actor.lock();
  }
};
} // namespace editor