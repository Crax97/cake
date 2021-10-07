#pragma once

#include <memory>

namespace renderer {
class texture;

/*
    A framebuffer is a region of memory where stuff can be rendered.
    By default a frame buffer contains a color texture and a depth texture
*/
class framebuffer {
public:
  virtual void bind() noexcept = 0;
  virtual void unbind() noexcept = 0;

  virtual std::shared_ptr<texture> get_color_texture() noexcept = 0;
  virtual std::shared_ptr<texture> get_depth_texture() noexcept = 0;
  virtual ~framebuffer() = default;
};
} // namespace renderer