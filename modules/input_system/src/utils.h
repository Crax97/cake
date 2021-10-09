#pragma once

#include "SDL_mouse.h"
#include "input/keys.h"

#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>

namespace inputsystem::utils {

inline SDL_KeyCode keyboard_key_to_sdl_key(keyboard_key key) noexcept {
  return static_cast<SDL_KeyCode>(key);
}
inline keyboard_key sDl_key_to_keyboard_key(SDL_KeyCode key) noexcept {
  return static_cast<keyboard_key>(key);
}
inline int mouse_button_to_sdl_mouse(mouse_button key) noexcept {
  return static_cast<int>(key);
}
inline mouse_button sdl_mouse_to_mouse_button(int key) noexcept {
  if (key >= static_cast<int>(mouse_button::Unknown)) {
    return mouse_button::Unknown;
  }
  return static_cast<mouse_button>(key);
}

} // namespace inputsystem::utils