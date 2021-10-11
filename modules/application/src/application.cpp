#include "application.h"

#include <cassert>

#include "SDL_events.h"
#include "api_factory.h"
#include "graphics_api.h"
#include "input/inputsystem.h"
#include "window.h"

static app_framework::application *g_the_application{nullptr};
app_framework::application::application(int argc, char **argv) noexcept {
  assert(!g_the_application);
  g_the_application = this;
}

void app_framework::application::setup() noexcept {
  m_window = std::make_unique<renderer::window>("Basic Application", m_width,
                                                m_height);
  m_api = renderer::api_factory::create_api("opengl", *m_window);

  inputsystem::inputsystem::init();
}

void app_framework::application::update_events() noexcept {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    on_sdl_event(event);

    switch (event.type) {
    case SDL_QUIT:
      exit(0);
      break;
    }
  }
}

void app_framework::application::shutdown() noexcept {
  inputsystem::inputsystem::shutdown();
}

void app_framework::application::update_inputs() noexcept {
  inputsystem::inputsystem::update();
  if (inputsystem::inputsystem::is_button_just_pressed(
          inputsystem::keyboard_key::Escape)) {
    exit(0);
  }
}

int app_framework::application::loop() noexcept {
  setup();

  on_app_startup();
  while (m_is_running) {
    update_events();
    update_inputs();

    m_api->pre_draw();
    on_app_update();
    m_api->post_draw();
  }

  on_app_shutdown();
  shutdown();

  return m_exit_code;
}

void app_framework::application::exit(int exit_code) {
  m_is_running = false;
  m_exit_code = exit_code;
}

app_framework::application &app_framework::application::the() noexcept {
  return *g_the_application;
}
renderer::window &app_framework::application::get_window() noexcept {
  return *m_window;
}
renderer::graphics_api &app_framework::application::get_api() noexcept {
  return *m_api;
}
app_framework::application::~application() noexcept {}