#include "opengl_shader.h"
#include "opengl_texture.h"

#include <cassert>
#include <glad/glad.h>
#include <iostream>

renderer::opengl_shader::opengl_shader(GLuint vs, GLuint fs) noexcept {
  m_program = glCreateProgram();
  glAttachShader(m_program, vs);
  glAttachShader(m_program, fs);

  glBindAttribLocation(m_program, KNOWN_LOCATIONS::VertexPosition, "vertex");
  glBindAttribLocation(m_program, KNOWN_LOCATIONS::TexcoordPosition,
                       "texcoord");
  glBindAttribLocation(m_program, KNOWN_LOCATIONS::NormalPosition, "normal");

  glLinkProgram(m_program);

  GLint stat = GL_TRUE;
  glGetProgramiv(m_program, GL_LINK_STATUS, &stat);
  if (stat != GL_TRUE) {
    char buf[2049];
    GLint len;
    glGetProgramInfoLog(m_program, 2048, &len, buf);
    std::cout << "Error compiling shading program " << m_program
              << std::string(buf, len) << "\n";
    glDeleteProgram(m_program);
    m_program = 0;
  }

  GLuint uniform_block_location =
      glGetUniformBlockIndex(m_program, "GlobalData");
  if (uniform_block_location != GL_INVALID_INDEX) {
    glUniformBlockBinding(m_program, uniform_block_location, 0);
  }
}

void renderer::opengl_shader::use() {
  if (m_program != 0)
    glUseProgram(m_program);
}

void renderer::opengl_shader::set_uniform_matrix4x4(
    const std::string &uniform_name, float *mat4x4) {
  auto uniform_location = get_uniform_location(uniform_name);
  if (uniform_location == -1) {
    // log error
    return;
  }
  glProgramUniformMatrix4fv(m_program, uniform_location, 1, GL_FALSE, mat4x4);
}

void renderer::opengl_shader::set_uniform_float(const std::string &uniform_name,
                                                float value) {
  auto uniform_location = get_uniform_location(uniform_name);
  if (uniform_location == -1) {
    // log error
    return;
  }
  glProgramUniform1f(m_program, uniform_location, value);
}

void renderer::opengl_shader::set_uniform_vec3(const std::string &uniform_name,
                                               float *vec) {
  auto uniform_location = get_uniform_location(uniform_name);
  if (uniform_location == -1) {
    // log error
    return;
  }
  auto diff_loc = get_uniform_location("diffuse");
  glProgramUniform3fv(m_program, uniform_location, 1, vec);
}

void renderer::opengl_shader::bind_textures(
    std::unordered_map<std::string, std::shared_ptr<class texture>> textures) {

  int i = 0;
  for (auto &pair : textures) {
    const auto &[unif_name, texture] = pair;
    auto unif_loc = get_uniform_location(unif_name);
    if (unif_loc == -1) {
      // log error
      continue;
    }
    texture->bind_to_unit(i);
    glProgramUniform1i(m_program, unif_loc, i);
  }
}
GLint renderer::opengl_shader::get_uniform_location(
    const std::string &uniform_name) noexcept {
  auto cached_uniform = uniforms.find(uniform_name);
  if (cached_uniform != uniforms.end()) {
    return cached_uniform->second;
  }

  auto uniform = glGetUniformLocation(m_program, uniform_name.c_str());
  uniforms.insert({uniform_name, uniform});
  return uniform;
}

renderer::opengl_shader::~opengl_shader() noexcept {
  glDeleteProgram(m_program);
}