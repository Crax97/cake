#pragma once

#include "mesh.h"
#include <glad/glad.h>

namespace renderer {
class opengl_mesh : public mesh {
public:
  void add_face(const face &face) noexcept override;

  void draw() const noexcept override;

  void bind() const noexcept override;
  size_t element_count() const noexcept override;
  opengl_mesh(const vertices &verts, const tex_coords &texs,
              const normals &norms, const indices &ind) noexcept;
  ~opengl_mesh() noexcept override;

private:
  GLuint vertex_buffer{0};
  GLuint texcoord_buffer{0};
  GLuint normal_buffer{0};
  GLuint element_buffer{0};

  GLuint mesh_vao{};

  int m_primitives_count;
};
} // namespace renderer