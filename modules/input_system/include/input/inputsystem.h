#pragma once

#include "keys.h"
#include <memory>

namespace inputsystem {
class inputsystem {
public:
  static void init() noexcept;
  static void update() noexcept;
  static void shutdown() noexcept;

  static button_state get_button_state(keyboard_key key) noexcept;
  static inline bool is_button_down(keyboard_key key) noexcept {
    return get_button_state(key) == button_state::Down;
  }
  static inline bool is_button_up(keyboard_key key) noexcept {
    return get_button_state(key) == button_state::Up;
  }
  static inline bool is_button_just_pressed(keyboard_key key) noexcept {
    return get_button_state(key) == button_state::JustPressed;
  }
  static inline bool is_button_just_released(keyboard_key key) noexcept {
    return get_button_state(key) == button_state::JustReleased;
  }
  ~inputsystem();
};
} // namespace inputsystem