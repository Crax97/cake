#pragma once

#include "graphics_api.h"
#include <SDL2/SDL_video.h>
#include <glad/glad.h>

namespace renderer {

class opengl_graphics_api final : public graphics_api {
private:
  SDL_GLContext GLContext;
  SDL_Window *Window;
  GLuint m_unif_block_buffer{};
  void resize_uniform_buffer(const size_t &buffer_size);

public:
  explicit opengl_graphics_api(SDL_Window *Window) noexcept;

  std::shared_ptr<instanced_mesh>
  create_instanced_mesh(const std::shared_ptr<mesh> &mesh_template,
                        std::unique_ptr<renderer::instance_descriptor>
                            &&descriptor) noexcept override;
  std::shared_ptr<mesh> create_mesh(vertices verts, indices ind, normals norms,
                                    tex_coords texs) noexcept override;
  std::shared_ptr<shader>
  compile_shader(const std::string &vertex_shader_src,
                 const std::string &pixel_shader_src) noexcept override;
  std::shared_ptr<class texture>
  create_texture(unsigned char *data, int width, int height,
                 texture_format format) noexcept override;

  std::shared_ptr<class framebuffer>
  create_framebuffer(int width, int height) noexcept override;
  int get_viewport_width() const noexcept override;
  int get_viewport_height() const noexcept override;

  void pre_draw() noexcept override;
  void post_draw() noexcept override;

  void *map_constant_buffer_impl(const size_t buffer_size) override;
  void unmap_constant_buffer() override;

  ~opengl_graphics_api() noexcept override;
};
} // namespace renderer