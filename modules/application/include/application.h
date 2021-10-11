#pragma once

#include <memory>

union SDL_Event;

namespace renderer {
class window;
class graphics_api;
} // namespace renderer

namespace app_framework {
class application {

private:
  std::unique_ptr<renderer::graphics_api> m_api{nullptr};
  std::unique_ptr<renderer::window> m_window{nullptr};

  bool m_is_running{true};
  int m_exit_code{0};

  void setup() noexcept;
  void shutdown() noexcept;

  void update_events() noexcept;
  void update_inputs() noexcept;

protected:
  int m_width{1024};
  int m_height{720};

public:
  application(int argc, char **argv) noexcept;

  int loop() noexcept;

  virtual void on_sdl_event(SDL_Event &event) noexcept {}
  virtual void on_app_startup() noexcept {}
  virtual void on_app_update() noexcept {}
  virtual void on_app_shutdown() noexcept {}

  static application &the() noexcept;
  renderer::window &get_window() noexcept;
  renderer::graphics_api &get_api() noexcept;

  void exit(int exit_code = 0);
  virtual ~application() noexcept;
};
} // namespace app_framework