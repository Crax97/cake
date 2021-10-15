#pragma once

#include <SDL2/SDL.h>
namespace renderer {
void Init() noexcept { SDL_Init(SDL_INIT_VIDEO); }
void Quit() noexcept { SDL_Quit(); }
} // namespace renderer