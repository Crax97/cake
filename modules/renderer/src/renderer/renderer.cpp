#include "renderer/renderer.hpp"

#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/trigonometric.hpp"
#include "graphics_api.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

void renderer::renderer::init() {
  m_quad_mesh = m_api.create_mesh(
      {
          {-1.0f, 1.0f, 0.0f},
          {1.0f, 1.0f, 0.0f},
          {-1.0f, -1.0f, 0.0f},
          {1.0f, -1.0f, 0.0f},
      },
      {0, 1, 2, 1, 2, 3}, {{0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1}},
      {{.0f, 1.0f}, {1.0f, 1.0f}, {.0f, .0f}, {1.0f, .0f}});

  auto vs_src = R"=(
#version 460 core

in vec3 vertex;
in vec2 texcoord;
in vec3 normal;

out vec2 fs_texcoord;
uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(vertex, 1.0);
    fs_texcoord = texcoord;
    fs_texcoord.y = 1.0f - fs_texcoord.y;
}
)=";
  auto fs_src = R"=(
#version 460 core

in vec2 fs_texcoord;

uniform sampler2D diffuse;

out vec4 color;

void main() {
    color = texture(diffuse, fs_texcoord);
}
)=";
  m_default_shader = m_api.compile_shader(vs_src, fs_src);
}

void renderer::renderer::begin_scene(
    const glm::mat4 &view_projection) noexcept {
  m_view_projection = view_projection;
}

void renderer::renderer::draw_mesh(
    std::shared_ptr<mesh> &mesh,
    const mesh_draw_configuration &config) noexcept {
  auto pass_shader = m_default_shader;
  if (config.draw_shader) {
    pass_shader = config.draw_shader;
  }
  auto model = glm::translate(config.location) * glm::scale(config.scale) *
               glm::toMat4(config.rotation);
  auto mvp = m_view_projection * model;
  pass_shader->use();
  pass_shader->set_uniform_matrix4x4("mvp", &mvp[0][0]);
  pass_shader->bind_textures(config.textures);
  mesh->draw();
}
void renderer::renderer::draw_quad(
    const mesh_draw_configuration &config) noexcept {
  draw_mesh(m_quad_mesh, config);
}

void renderer::renderer::draw_texture(
    std::shared_ptr<texture> &texture,
    const texture_draw_configuration &configuration) noexcept {

  draw_quad(
      {.location = glm::vec3(configuration.location, configuration.z_index),
       .scale = glm::vec3(configuration.scale, 1.0),
       .rotation = glm::toQuat(glm::rotate(glm::radians(configuration.rotation),
                                           glm::vec3(0, 0, 1))),
       .textures = {{"diffuse", texture}},
       .draw_shader = configuration.draw_shader ? configuration.draw_shader
                                                : m_default_shader,
       .wireframe = configuration.wireframe});
}
void renderer::renderer::flush() noexcept {
  // This is unimplemented now
  // It's going to be implemented when we add support for batched rendering
}
