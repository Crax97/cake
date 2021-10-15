#pragma once

#include "framebuffer.h"
#include "glad/glad.h"

#include <memory>

namespace renderer {
class opengl_graphics_api;
class opengl_texture;
class texture;
class opengl_framebuffer final : public framebuffer {
private:
  GLuint m_framebuffer_name;
  opengl_graphics_api &m_api;
  std::shared_ptr<renderer::opengl_texture> m_color_texture;
  std::shared_ptr<renderer::opengl_texture> m_depth_texture;

public:
  explicit opengl_framebuffer(opengl_graphics_api &api, int width, int height);
  void bind() noexcept override;
  void unbind() noexcept override;
  std::shared_ptr<class texture> get_color_texture() noexcept override;
  std::shared_ptr<class texture> get_depth_texture() noexcept override;
  ~opengl_framebuffer();
};
} // namespace renderer