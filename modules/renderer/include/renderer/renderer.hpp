#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace renderer {
class graphics_api;
class mesh;
class texture;
class shader;
} // namespace renderer

namespace renderer {
struct mesh_draw_configuration {
  glm::vec3 location;
  glm::vec3 scale = glm::vec3(1);
  glm::quat rotation = glm::identity<glm::quat>();
  std::unordered_map<std::string, std::shared_ptr<texture>> textures;
  std::shared_ptr<shader> draw_shader;
  bool wireframe = false;
};

struct texture_draw_configuration {
  glm::vec2 location;
  glm::vec2 scale = glm::vec2(1);
  float rotation = 0;
  std::shared_ptr<shader> draw_shader;
  int z_index = 0;
  bool wireframe = false;
};
class renderer {
private:
  graphics_api &m_api;
  std::shared_ptr<mesh> m_quad_mesh;
  std::shared_ptr<shader> m_default_shader;
  glm::mat4 m_view_projection;
  void init();

public:
  renderer(graphics_api &api) noexcept : m_api(api) { init(); }

  void begin_scene(const glm::mat4 &view_projection) noexcept;
  void draw_mesh(std::shared_ptr<mesh> &mesh,
                 const mesh_draw_configuration &config) noexcept;
  void draw_quad(const mesh_draw_configuration &config) noexcept;

  void draw_texture(std::shared_ptr<texture> &texture,
                    const texture_draw_configuration &configuration) noexcept;
  void flush() noexcept;
};
} // namespace renderer