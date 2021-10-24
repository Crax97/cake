#pragma once

#include <memory>

namespace spectacle {
class stage;
class actor;
} // namespace spectacle

class editor_application;

namespace editor {
class actor_editor {
private:
  editor_application &m_editor;
  std::weak_ptr<spectacle::actor> m_selected_actor;

public:
  constexpr actor_editor(editor_application &application) noexcept
      : m_editor(application) {}
  void draw_editor() noexcept;
  void
  show_actor_fields(const std::shared_ptr<spectacle::actor>& the_actor) noexcept;
  void
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