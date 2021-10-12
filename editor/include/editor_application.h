#pragma once

#include "application.h"
#include "game_framework/game.h"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl.h"
#include "glm/glm.hpp"

struct ImGuiContext;

class editor_application : public gameframework::game {

private:
  const int sidebar_width = 200; // px
  int menubar_height = 0;

  bool m_level_is_dirty = false;
  bool m_show_demo = false;
  bool m_file_menu = false;

  ImGuiContext *m_imgui_context{nullptr};
  glm::vec2 viewport_size{800, 600};

  void draw_menubar() noexcept;
  void draw_sidebar() noexcept;
  void update_imgui() noexcept;

  void on_new_level() noexcept;
  void on_load_request() noexcept;
  void on_save_request() noexcept;

public:
  editor_application(int argc, char **argv);

  void on_sdl_event(SDL_Event &event) noexcept override {
    ImGui_ImplSDL2_ProcessEvent(&event);
  }
  void on_app_startup() noexcept override;
  void on_app_update() noexcept override;
  void on_app_shutdown() noexcept override;

  ~editor_application() noexcept;
};