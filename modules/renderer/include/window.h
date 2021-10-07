#pragma once

#include <string>

struct SDL_Window;

namespace renderer {
class window {
private:
  SDL_Window *SDLWin;
  friend class api_factory;

public:
  struct relative_coords {
    float x;
    float y;
  };

  window(const std::string &title, int width, int height,
         bool fullscreen = false) noexcept;

  int GetWidth() const noexcept;
  int GetHeight() const noexcept;
  void SetWidth(int NewWidth) noexcept;
  void SetHeight(int NewHeight) noexcept;

  bool IsFullscreen() const noexcept;
  void SetNewFullscreen(bool NewFullscreen) noexcept;
  void set_title(const std::string &title) noexcept;
  void set_mouse_location(int new_x, int new_y) noexcept;
  void get_mouse_location(int &x, int &y) noexcept;
  relative_coords window_to_relative(int x, int y) const noexcept;

  void set_show_cursor(bool value) noexcept;
  bool get_show_cursor() noexcept;

  ~window() noexcept;

protected:
  SDL_Window *GetSDLWindow() noexcept;
};
} // namespace renderer