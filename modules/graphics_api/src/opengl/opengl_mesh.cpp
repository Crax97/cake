#include <logging/logger.h>
#include "opengl_mesh.h"
#include "glad/glad.h"
#include "opengl_shader.h"

void renderer::opengl_mesh::add_face(const renderer::face &face) noexcept {}

void renderer::opengl_mesh::draw() const noexcept {
  bind();
  glDrawElements(GL_TRIANGLES, m_primitives_count, GL_UNSIGNED_INT, nullptr);
}

template <GLenum Buf_Type, typename D>
GLuint gen_typed_buffer(const std::vector<D> &data) {
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(Buf_Type, buffer);
  glBufferData(Buf_Type, data.size() * sizeof(D), data.data(), GL_STATIC_DRAW);
  return buffer;
}

renderer::opengl_mesh::opengl_mesh(const renderer::vertices &verts,
                                   const renderer::tex_coords &texs,
                                   const renderer::normals &norms,
                                   const renderer::indices &ind) noexcept {
  glGenVertexArrays(1, &mesh_vao);
  glBindVertexArray(mesh_vao);

  vertex_buffer = gen_typed_buffer<GL_ARRAY_BUFFER>(verts);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glEnableVertexAttribArray(opengl_shader::KNOWN_LOCATIONS::VertexPosition);
  glVertexAttribPointer(opengl_shader::KNOWN_LOCATIONS::VertexPosition, 3,
                        GL_FLOAT, GL_FALSE, 0, nullptr);

  if (!texs.empty()) {
    texcoord_buffer = gen_typed_buffer<GL_ARRAY_BUFFER>(texs);
    glBindBuffer(GL_ARRAY_BUFFER, texcoord_buffer);
    glEnableVertexAttribArray(opengl_shader::KNOWN_LOCATIONS::TexcoordPosition);
    glVertexAttribPointer(opengl_shader::KNOWN_LOCATIONS::TexcoordPosition, 2,
                          GL_FLOAT, GL_FALSE, 0, nullptr);
  }
  if (!norms.empty()) {
    normal_buffer = gen_typed_buffer<GL_ARRAY_BUFFER>(norms);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glEnableVertexAttribArray(opengl_shader::KNOWN_LOCATIONS::NormalPosition);
    glVertexAttribPointer(opengl_shader::KNOWN_LOCATIONS::NormalPosition, 3,
                          GL_FLOAT, GL_FALSE, 0, nullptr);
  }

  element_buffer = gen_typed_buffer<GL_ELEMENT_ARRAY_BUFFER>(ind);
  m_primitives_count = ind.size();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);

  glBindVertexArray(0);
}

renderer::opengl_mesh::~opengl_mesh() noexcept {
  glDisableVertexArrayAttrib(mesh_vao,
                             opengl_shader::KNOWN_LOCATIONS::VertexPosition);
  glDisableVertexArrayAttrib(mesh_vao,
                             opengl_shader::KNOWN_LOCATIONS::TexcoordPosition);
  glDisableVertexArrayAttrib(mesh_vao,
                             opengl_shader::KNOWN_LOCATIONS::NormalPosition);
  glDeleteBuffers(1, &vertex_buffer);
  glDeleteBuffers(1, &element_buffer);
  if (texcoord_buffer != 0)
    glDeleteBuffers(1, &texcoord_buffer);
  if (normal_buffer != 0)
    glDeleteBuffers(1, &normal_buffer);

    MIKU_ASSERT(!glIsBuffer(vertex_buffer) && !glIsBuffer(texcoord_buffer) &&
         !glIsBuffer(normal_buffer) && !glIsBuffer(element_buffer));

  glDeleteVertexArrays(1, &mesh_vao);
}

void renderer::opengl_mesh::bind() const noexcept {
  glBindVertexArray(mesh_vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
}
size_t renderer::opengl_mesh::element_count() const noexcept {
  return m_primitives_count;
}