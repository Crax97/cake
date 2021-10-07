#include "api_factory.h"

#include "graphics_api.h"
#include "window.h"

#include <functional>
#include <unordered_map>
#include <utility>

#ifdef OPENGL_FOUND
#include "opengl/opengl_graphics_api.h"
#endif
using api_creation_fn =
    std::function<std::unique_ptr<renderer::graphics_api>(SDL_Window *)>;

std::unordered_map<std::string, api_creation_fn> build_api_map() {
  std::unordered_map<std::string, api_creation_fn> renderer_creators;
#ifdef OPENGL_FOUND
  renderer_creators.insert(std::make_pair<std::string, api_creation_fn>(
      std::string("opengl"), [](SDL_Window *Win) {
        return std::make_unique<renderer::opengl_graphics_api>(Win);
      }));
#endif
  return renderer_creators;
}

std::unique_ptr<renderer::graphics_api>
renderer::api_factory::create_api(const std::string &preferred_renderer,
                                  window &Win) noexcept {
  static auto renderer_map = build_api_map();
  auto renderer_fn = renderer_map.find(preferred_renderer);
  if (renderer_fn != renderer_map.end()) {
    return renderer_fn->second(Win.GetSDLWindow());
  }
  return nullptr;
}