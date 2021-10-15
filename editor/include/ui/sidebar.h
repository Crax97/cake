#pragma once

namespace spectacle {
class stage;
}

class editor_application;

namespace editor {
class sidebar {
private:
  editor_application &m_editor;

public:
  constexpr sidebar(editor_application &application) noexcept
      : m_editor(application) {}
  void draw_sidebar() noexcept;
};
} // namespace editor