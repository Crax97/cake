#include "opengl_texture.h"

#include <cassert>
#include <glad/glad.h>

renderer::opengl_texture::opengl_texture(GLuint tex_id, int width,
                                         int height) noexcept
    : texture(width, height), m_texture(tex_id) {
  assert(glIsTexture(tex_id));
}

renderer::opengl_texture::~opengl_texture() noexcept {
  glDeleteTextures(1, &m_texture);
}

void renderer::opengl_texture::bind_to_unit(int unit) {
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glActiveTexture(GL_TEXTURE0 + unit);
}

bool renderer::opengl_texture::has_valid_texture() const {
    return m_texture != 0;
}
