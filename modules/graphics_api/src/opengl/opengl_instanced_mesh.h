#pragma once
#include "instanced_mesh.h"

#include "opengl_mesh.h"

#include "glad/glad.h"

namespace renderer {
class opengl_instanced_mesh final : public instanced_mesh {
private:
  GLuint mesh_instance_buffer;
  GLuint m_associated_program{0};
  size_t current_buffer_size{0};
  size_t current_element_count{0};

  void ensure_buffer_has_enough_size(size_t new_size) noexcept;

protected:
  void add_instance_impl(void *instance_ptr) noexcept override;
  void *map_instance_buffer_impl(const size_t elem_count) noexcept override;

public:
  opengl_instanced_mesh(std::shared_ptr<renderer::mesh> mesh,
                        std::unique_ptr<instance_descriptor> &&descriptor);
  void unmap_instance_buffer() override;
  bool configure_for_shader(
      std::shared_ptr<class shader> requiredShader) noexcept override;
  void add_face(const face &face) noexcept override {}
  void clear() noexcept override;
  void draw() const noexcept override;
  void draw(int instance_count) const noexcept;
};
} // namespace renderer