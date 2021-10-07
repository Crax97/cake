#include "opengl_framebuffer.h"
#include "glad/glad.h"
#include "opengl_graphics_api.h"
#include "opengl_texture.h"
#include "texture.h"

#include <assert.h>
#include <glad/glad.h>
#include <memory>

void ensure_framebuffer_ok(GLenum status) {
  // TODO: Do more explicit checks
  assert(status == GL_FRAMEBUFFER_COMPLETE);
}

renderer::opengl_framebuffer::opengl_framebuffer(
    renderer::opengl_graphics_api &api, int width, int height)
    : m_api(api) {
  m_color_texture =
      std::dynamic_pointer_cast<opengl_texture>(m_api.create_texture(
          nullptr, width, height, renderer::texture_format::rgba));
  m_depth_texture =
      std::dynamic_pointer_cast<opengl_texture>(m_api.create_texture(
          nullptr, width, height, renderer::texture_format::depth_texture));
  assert(m_color_texture != nullptr && m_depth_texture != nullptr);

  glGenFramebuffers(1, &m_framebuffer_name);
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_name);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                       m_color_texture->get_texture(), 0);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                       m_depth_texture->get_texture(), 0);

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  ensure_framebuffer_ok(status);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderer::opengl_framebuffer::bind() noexcept {
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_name);
  glViewport(0, 0, m_color_texture->get_width(), m_color_texture->get_height());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer::opengl_framebuffer::unbind() noexcept {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, m_api.get_viewport_width(), m_api.get_viewport_height());
}

std::shared_ptr<renderer::texture>
renderer::opengl_framebuffer::get_color_texture() noexcept {
  return m_color_texture;
}

std::shared_ptr<renderer::texture>
renderer::opengl_framebuffer::get_depth_texture() noexcept {
  return m_depth_texture;
}
renderer::opengl_framebuffer::~opengl_framebuffer() {
  glDeleteFramebuffers(1, &m_framebuffer_name);
}