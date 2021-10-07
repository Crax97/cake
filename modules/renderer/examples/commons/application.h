#pragma once

#include "Init.h"
#include "SDL_mouse.h"
#include "api_factory.h"
#include "graphics_api.h"
#include "window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include <memory>

namespace example_support_library {
class application {
private:
  std::unique_ptr<renderer::window> m_win;
  std::unique_ptr<renderer::graphics_api> m_api;

  int m_return_code = 0;
  bool m_running = true;
  float m_total_time = 0.0f;
  float m_delta_time = 0.0f;

  int m_posx{0}, m_posy{0};

  std::string m_name;
  int m_window_width;
  int m_window_height;

  void init_api() noexcept {
    m_win = std::make_unique<renderer::window>(m_name, m_window_width,
                                               m_window_height);
    m_api = renderer::api_factory::create_api("opengl", *m_win);
  }

  void poll_events() {
    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
      switch (evt.type) {
      case SDL_QUIT:
        app_exit(0);
        break;
      case SDL_KEYDOWN:
        on_app_keydown(evt.key);
        break;
      case SDL_KEYUP:
        on_app_keyup(evt.key);
        break;
      case SDL_MOUSEBUTTONDOWN:
        on_app_mousedown(evt.button);
        break;
      case SDL_MOUSEBUTTONUP:
        on_app_mouseup(evt.button);
        break;
      }
    }
  }

  void handle_mouse_motion() {
    int pos_x, pos_y;
    m_win->get_mouse_location(pos_x, pos_y);

    if (pos_x > 0 || pos_y > 0) {
      on_app_mousemove(pos_x, -pos_y);
    }
    if (!m_win->get_show_cursor())
      m_win->set_mouse_location(0, 0);
  }

public:
  application(std::string name, int window_width, int window_height)
      : m_name(std::move(name)), m_window_width(window_width),
        m_window_height(window_height) {}

  int loop() {

    renderer::Init();
    init_api();
    on_app_startup();

    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = 0;

    while (m_running) {
      last = now;

      poll_events();
      handle_mouse_motion();
      on_app_loop(m_delta_time);

      now = SDL_GetPerformanceCounter();
      m_delta_time = static_cast<float>(now - last) /
                     static_cast<float>(SDL_GetPerformanceFrequency());
      m_total_time += m_delta_time;
    }

    on_app_shutdown();
    renderer::Quit();
    return m_return_code;
  }

  std::unique_ptr<renderer::graphics_api> &get_graphics_api() noexcept {
    return m_api;
  }

  std::unique_ptr<renderer::window> &get_window() noexcept { return m_win; }

  void set_title(const std::string &title) noexcept { m_win->set_title(title); }

  void app_exit(int return_code) {
    m_running = false;
    m_return_code = return_code;
  }

  float get_total_time() { return m_total_time; }
  float get_delta_time() { return m_delta_time; }

  virtual void on_app_startup() {}
  virtual void on_app_loop(float delta_seconds) {}
  virtual void on_app_shutdown() {}

  virtual void on_app_keydown(const SDL_KeyboardEvent &event) {}
  virtual void on_app_keyup(const SDL_KeyboardEvent &event) {}
  virtual void on_app_mousemove(const int delta_x, const int delta_y) {}
  virtual void on_app_mousedown(const SDL_MouseButtonEvent &event) {}
  virtual void on_app_mouseup(const SDL_MouseButtonEvent &event) {}
};
} // namespace example_support_library