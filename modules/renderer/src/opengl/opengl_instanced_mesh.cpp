#include "opengl_instanced_mesh.h"
#include "shader.h"

#include "glad/glad.h"
#include <cassert>
#include <cstring>
#include <memory>
#include <stdio.h>

struct gl_attrib_format {
  GLenum type;
  GLuint element_size;
  GLuint elements_per_location;
  GLuint num_locations;
};
gl_attrib_format
gl_description_from_descriptor(const renderer::attrib_descriptor &descriptor) {
  GLenum el_type;
  GLuint single_element_size = 0;
  GLuint elements_count = 0;
  GLuint locations_used = 1;
  switch (descriptor.type) {
  case renderer::attrib_data_type::Byte:
    single_element_size = 1;
    el_type = GL_BYTE;
    break;
  case renderer::attrib_data_type::Unsigned_Byte:
    el_type = GL_UNSIGNED_BYTE;
    single_element_size = 1;
    break;
  case renderer::attrib_data_type::Unsigned_Int:
    el_type = GL_UNSIGNED_INT;
    single_element_size = 4;
    break;
  case renderer::attrib_data_type::Int:
    el_type = GL_INT;
    single_element_size = 4;
    break;
  case renderer::attrib_data_type::Float:
    el_type = GL_FLOAT;
    single_element_size = 4;
    break;
  case renderer::attrib_data_type::Double:
    el_type = GL_DOUBLE;
    single_element_size = 8;
    break;
  }
  switch (descriptor.el_count) {
  case renderer::attrib_element_count::One:
    elements_count = 1;
    break;
  case renderer::attrib_element_count::Two:
    elements_count = 2;
    break;
  case renderer::attrib_element_count::Three:
    elements_count = 3;
    break;
  case renderer::attrib_element_count::Four:
    elements_count = 4;
    break;
  case renderer::attrib_element_count::Mat2:
    elements_count = 4;
    break;
  case renderer::attrib_element_count::Mat3:
    elements_count = 3;
    locations_used = 3;
    break;
  case renderer::attrib_element_count::Mat4:
    elements_count = 4;
    locations_used = 4;
    break;
  }
  return {el_type, single_element_size, elements_count, locations_used};
}

renderer::opengl_instanced_mesh::opengl_instanced_mesh(
    std::shared_ptr<renderer::mesh> mesh,
    std::unique_ptr<renderer::instance_descriptor> &&descriptor)
    : instanced_mesh(mesh, std::move(descriptor)) {
  m_mesh->bind();
  glGenBuffers(1, &mesh_instance_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, mesh_instance_buffer);
}

void renderer::opengl_instanced_mesh::ensure_buffer_has_enough_size(
    size_t new_size) noexcept {
  const auto factor = 1.5f;
  if (current_buffer_size < new_size) {
    const auto scaled_size = static_cast<GLuint>(new_size * factor);
    glBindBuffer(GL_ARRAY_BUFFER, mesh_instance_buffer);
    glBufferData(GL_ARRAY_BUFFER, scaled_size, nullptr, GL_STREAM_DRAW);
    current_buffer_size = scaled_size;
  }
}

void renderer::opengl_instanced_mesh::add_instance_impl(
    void *instance_ptr) noexcept {
  const auto size = m_descriptor->get_element_size();
  const auto offset = current_element_count * size;
  ensure_buffer_has_enough_size((current_element_count + 1) * size);
  glBindBuffer(GL_ARRAY_BUFFER, mesh_instance_buffer);
  glBufferSubData(GL_ARRAY_BUFFER, offset, size, instance_ptr);
  current_element_count++;
}

void renderer::opengl_instanced_mesh::unmap_instance_buffer() {
  glUnmapBuffer(GL_ARRAY_BUFFER);
}
void *renderer::opengl_instanced_mesh::map_instance_buffer_impl(
    const size_t elem_count) noexcept {
  ensure_buffer_has_enough_size(elem_count * m_descriptor->get_element_size());
  if (current_element_count < elem_count) {
    current_element_count = elem_count;
  }
  glBindBuffer(GL_ARRAY_BUFFER, mesh_instance_buffer);
  auto *mapped = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
  return mapped;
}

bool renderer::opengl_instanced_mesh::configure_for_shader(
    std::shared_ptr<class renderer::shader> requiredShader) noexcept {
  m_mesh->bind();
  requiredShader->use();
  glBindBuffer(GL_ARRAY_BUFFER, mesh_instance_buffer);

  GLint bound_program;
  glGetIntegerv(GL_CURRENT_PROGRAM, &bound_program);
  if (bound_program == -1)
    // No program is currently bound, rip
    return false;

  glUseProgram(0);

  m_associated_program = bound_program;
  for (const auto &descriptor : m_descriptor->get_descriptors()) {
    GLuint attrib_base_location = glGetAttribLocation(
        m_associated_program, descriptor.attrib_name.data());
    if (attrib_base_location == GL_INVALID_INDEX)
      continue;

    const auto [type, el_size, el_count, el_locations] =
        gl_description_from_descriptor(descriptor);
    for (GLuint i = 0; i < el_locations; i++) {
      // 1. Enable attrib for instancing
      const GLuint attrib_location = attrib_base_location + i;
      const GLuint stride = m_descriptor->get_stride();
      glEnableVertexAttribArray(attrib_location);
      // 2. Describe attrib
      glVertexAttribPointer(attrib_location, el_count, type, GL_FALSE, stride,
                            reinterpret_cast<void *>(descriptor.offset));
      glVertexAttribDivisor(attrib_location, 1);
    }
  }
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  return true;
}

void renderer::opengl_instanced_mesh::clear() noexcept {
  // reuse the instance buffer
  current_element_count = 0;
}

void renderer::opengl_instanced_mesh::draw() const noexcept {
  draw(current_element_count);
}
void renderer::opengl_instanced_mesh::draw(int instance_count) const noexcept {
  assert(instance_count <= current_element_count);
  glUseProgram(m_associated_program);
  m_mesh->bind();
  glDrawElementsInstanced(GL_TRIANGLES, m_mesh->element_count(),
                          GL_UNSIGNED_INT, nullptr, instance_count);
}