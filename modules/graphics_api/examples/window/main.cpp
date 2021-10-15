#include <SDL2/SDL_events.h>
#include <iostream>

#include "Init.h"
#include "window.h"

int main(int argc, char **argv) {
  renderer::Init();

  auto win = renderer::window("Sample window", 800, 600);
  bool running = true;
  while (running) {
    SDL_Event evt;
    SDL_PollEvent(&evt);
    switch (evt.type) {
    case SDL_QUIT:
      running = false;
      break;
    }
  }

  renderer::Quit();
  return 0;
}