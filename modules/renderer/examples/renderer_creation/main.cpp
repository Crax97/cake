#include <SDL2/SDL_events.h>

#include "Init.h"
#include "graphics_api.h"
#include "api_factory.h"
#include "window.h"

int main(int argc, char **argv) {
  renderer::Init();

  auto win = renderer::window("Sample window", 800, 600);
  auto renderer = renderer::api_factory::create_api("opengl", win);
  bool running = true;
  while (running) {
    SDL_Event evt;
    SDL_PollEvent(&evt);
    switch (evt.type) {
    case SDL_QUIT:
      running = false;
      break;
    }

      renderer->pre_draw();
  }

  renderer::Quit();
  return 0;
}