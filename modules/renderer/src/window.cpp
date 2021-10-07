#include "window.h"
#include "SDL_events.h"
#include "SDL_mouse.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>

#include <assert.h>

renderer::window::window(const std::string &title, int width, int height,
                         bool fullscreen) noexcept {

  Uint32 flags = 0;
#ifdef OPENGL_FOUND
  flags |= SDL_WINDOW_OPENGL;
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#endif
#ifdef VULKAN_FOUND
  flags |= SDL_WINDOW_VULKAN;
#endif
#ifdef METAL_FOUND
  flags |= SDL_WINDOW_METAL;
#endif
  if (fullscreen) {
    flags |= SDL_WINDOW_FULLSCREEN;
  }
  SDL_ShowCursor(false);

  this->SDLWin = SDL_CreateWindow(title.c_str(), 0, 0, width, height, flags);
  assert(this->SDLWin);
}

int renderer::window::GetWidth() const noexcept {
  assert(this->SDLWin);

  int w, h;
  SDL_GetWindowSize(this->SDLWin, &w, &h);
  return w;
}
int renderer::window::GetHeight() const noexcept {
  assert(this->SDLWin);

  int w, h;
  SDL_GetWindowSize(this->SDLWin, &w, &h);
  return h;
}
void renderer::window::SetWidth(int NewWidth) noexcept {
  assert(this->SDLWin);

  SDL_SetWindowSize(this->SDLWin, NewWidth, GetHeight());
}
void renderer::window::SetHeight(int NewHeight) noexcept {
  assert(this->SDLWin);

  SDL_SetWindowSize(this->SDLWin, GetWidth(), NewHeight);
}

bool renderer::window::IsFullscreen() const noexcept {
  assert(this->SDLWin);
  return SDL_GetWindowFlags(SDLWin) & SDL_WINDOW_FULLSCREEN;
}
void renderer::window::SetNewFullscreen(bool NewFullscreen) noexcept {
  assert(this->SDLWin);

  auto flags = SDL_GetWindowFlags(SDLWin);
  if (NewFullscreen) {
    flags |= SDL_WINDOW_FULLSCREEN;
  } else {
    flags &= ~SDL_WINDOW_FULLSCREEN;
  }
  SDL_SetWindowFullscreen(this->SDLWin, flags);
}

void renderer::window::set_title(const std::string &new_title) noexcept {
  assert(this->SDLWin);

  SDL_SetWindowTitle(SDLWin, new_title.c_str());
}

void renderer::window::set_mouse_location(int new_x, int new_y) noexcept {
  const int width = GetWidth();
  const int height = GetHeight();
  new_x = width / 2 + new_x;
  new_y = (height / 2 + new_y);
  SDL_WarpMouseInWindow(SDLWin, new_x, new_y);
}
void renderer::window::get_mouse_location(int &x, int &y) noexcept {
  const int width = GetWidth();
  const int height = GetHeight();
  SDL_GetMouseState(&x, &y);
  x = x - width / 2;
  y = (height / 2 - y);
}
renderer::window::relative_coords
renderer::window::window_to_relative(int x, int y) const noexcept {

  const float half_width = static_cast<float>(GetWidth()) / 2;
  const float half_height = static_cast<float>(GetHeight()) / 2;
  return {static_cast<float>(x) / half_width,
          -static_cast<float>(y) / half_height};
}

void renderer::window::set_show_cursor(bool value) noexcept {
  SDL_ShowCursor(value ? SDL_ENABLE : SDL_DISABLE);
}
bool renderer::window::get_show_cursor() noexcept {
  return SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE ? true : false;
}

renderer::window::~window() noexcept {
  if (SDLWin) {
    SDL_DestroyWindow(SDLWin);
    SDLWin = nullptr;
  }
}
SDL_Window *renderer::window::GetSDLWindow() noexcept {
  assert(this->SDLWin);
  return this->SDLWin;
}