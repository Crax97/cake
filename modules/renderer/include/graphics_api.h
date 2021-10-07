#pragma once

#include "instanced_mesh.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

#include <memory>

struct SDL_Window;

namespace renderer {

class graphics_api {
private:
  virtual void *map_constant_buffer_impl(const size_t buffer_size) = 0;

public:
  virtual ~graphics_api() noexcept = default;

  virtual std::shared_ptr<instanced_mesh> create_instanced_mesh(
      const std::shared_ptr<mesh> &mesh_template,
      std::unique_ptr<renderer::instance_descriptor> &&descriptor) noexcept = 0;
  virtual std::shared_ptr<mesh> create_mesh(vertices verts, indices ind,
                                            normals norms,
                                            tex_coords texs) noexcept = 0;
  virtual std::shared_ptr<shader>
  compile_shader(const std::string &vertex_shader_src,
                 const std::string &pixel_shader_src) noexcept = 0;
  virtual std::shared_ptr<class texture>
  create_texture(unsigned char *data, int width, int height,
                 texture_format format) noexcept = 0;
  virtual std::shared_ptr<class framebuffer>
  create_framebuffer(int width, int height) noexcept = 0;

  virtual int get_viewport_width() const noexcept = 0;
  virtual int get_viewport_height() const noexcept = 0;

  virtual void pre_draw() noexcept = 0;
  virtual void post_draw() noexcept = 0;

  template <typename T> T *map_constant_buffer() {
    return static_cast<T *>(map_constant_buffer_impl(sizeof(T)));
  }

  virtual void unmap_constant_buffer() = 0;
};
} // namespace renderer