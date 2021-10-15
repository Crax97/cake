#pragma once

#include <memory>
#include <string>

namespace renderer {
class window;
class api_factory {

public:
  static std::unique_ptr<class graphics_api>
  create_api(const std::string &preferred_renderer, window &Win) noexcept;
};
} // namespace renderer