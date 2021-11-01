#include "opengl_texture.h"

#include <glad/glad.h>
#include <logging/logger.h>

renderer::opengl_texture::opengl_texture(GLuint tex_id, int width,
                                         int height) noexcept
    : texture(width, height), m_texture(tex_id) {
    MIKU_ASSERT(glIsTexture(tex_id));
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
