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
  void select_actor(std::shared_ptr<spectacle::actor> new_actor);
};
} // namespace editor