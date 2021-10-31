#pragma once

#include <memory>

namespace spectacle {
class stage;
class actor;
} // namespace spectacle

class editor_application;

namespace editor {
class sidebar {
private:
  editor_application &m_editor;
  std::shared_ptr<spectacle::actor> m_selected_actor;
  bool m_is_editing_actor = false;
public:
  constexpr sidebar(editor_application &application) noexcept
      : m_editor(application) {}
  void draw_sidebar() noexcept;
};
} // namespace editor