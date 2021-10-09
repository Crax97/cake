#include "input/inputsystem.h"
#include "SDL_keyboard.h"
#include "utils.h"

#include <cstring>
#include <unordered_map>

namespace inputsystem {
class inputsystem_impl {
private:
  Uint8 m_last_frame_states[SDL_NUM_SCANCODES];
  button_state m_current_button_states[SDL_NUM_SCANCODES];

public:
  void init() {}
  void update() {
    auto *this_frame_states = SDL_GetKeyboardState(nullptr);

    for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
      m_current_button_states[i] = button_state::Up;
      if (this_frame_states[i] == 1 && m_last_frame_states[i] == 0) {
        m_current_button_states[i] = button_state::JustPressed;
      } else if (this_frame_states[i] == 1 && m_last_frame_states[i] == 1) {
        m_current_button_states[i] = button_state::Down;
      } else if (this_frame_states[i] == 0 && m_last_frame_states[i] == 1) {
        m_current_button_states[i] = button_state::JustReleased;
      }
    }

    memcpy(m_last_frame_states, this_frame_states, SDL_NUM_SCANCODES);
  }
  void shutdown() {}

  button_state get_button_state(keyboard_key key) const noexcept {
    return m_current_button_states[utils::keyboard_key_to_sdl_key(key)];
  }
};
inputsystem_impl m_impl;
} // namespace inputsystem

void inputsystem::inputsystem::init() noexcept { m_impl.init(); }
void inputsystem::inputsystem::update() noexcept { m_impl.update(); }
void inputsystem::inputsystem::shutdown() noexcept { m_impl.shutdown(); }
inputsystem::button_state
inputsystem::inputsystem::get_button_state(keyboard_key key) noexcept {
  return m_impl.get_button_state(key);
}

inputsystem::inputsystem::~inputsystem() {}