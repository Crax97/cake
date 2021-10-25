#pragma once

namespace renderer {
enum class texture_format { rgb, rgba, red, depth_texture, unknown };
class texture {
private:
  int m_width;
  int m_height;

public:
  texture(int width, int height) : m_width(width), m_height(height) {}
  virtual ~texture() = default;
  virtual void bind_to_unit(int unit) = 0;
  virtual void *get_texture_object() = 0;
  [[nodiscard]] virtual bool has_valid_texture() const = 0;
  [[nodiscard]] int get_width() const noexcept { return m_width; }
  [[nodiscard]] int get_height() const noexcept { return m_height; }
};
}; // namespace renderer