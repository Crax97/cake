#pragma once

#include <filesystem>
#include <optional>

namespace renderer {
class texture;
}

namespace gameframework {
class texture_loader {

public:
  static std::optional<std::shared_ptr<renderer::texture>>
  load_texture(const std::filesystem::path &path) noexcept;
};
} // namespace gameframework