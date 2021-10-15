
#include "Init.h"
#include "api_factory.h"
#include "texture.h"
#include "window.h"

#include "application.h"
#include "utilities.h"
#include <memory>

class cube_app : public example_support_library::application {
private:
  float m_rot_y = 0.0f;
  std::shared_ptr<renderer::mesh> cube_mesh;
  std::shared_ptr<renderer::mesh> quad_mesh;
  std::shared_ptr<renderer::texture> cube_texture;
  std::shared_ptr<renderer::shader> cube_shader;

  glm::mat4 compute_mvp(float delta_seconds) {
    auto &api = get_graphics_api();

    auto projection = glm::perspective(static_cast<float>(M_PI) * 0.7f,
                                       800.0f / 600.0f, 0.01f, 10000.0f);
    auto view = glm::lookAt(glm::vec3(0.0f, 30.0f, -30.0f), glm::vec3(0.0f),
                            glm::vec3(0, 1, 0));
    auto model =
        glm::scale(glm::vec3(10.0f)) * glm::rotate(m_rot_y, glm::vec3(0, 1, 0));
    auto mv = view * model;
    auto mvp = projection * mv;
    return mvp;
  }
  glm::mat4 compute_mvp_ortho(float delta_seconds) {
    auto projection =
        glm::ortho(-400.0f, 400.0f, -300.0f, 300.0f, 0.01f, 10000.0f);
    auto view = glm::mat4(1.0f);
    auto model = glm::translate(glm::vec3(-300.0f, -200.0f, -1.0f)) *
                 glm::scale(glm::vec3(100.0f));
    auto mv = view * model;
    auto mvp = projection * mv;
    return mvp;
  }

public:
  cube_app()
      : example_support_library::application("Cube application", 800, 600) {}
  void on_app_startup() override {

    auto &api = get_graphics_api();
    cube_mesh =
        utilities::mesh_load_from_path(api, "assets/models/cube.obj")[0];
    quad_mesh = utilities::create_quad(api);
    cube_shader =
        api->compile_shader(utilities::read_file("assets/shaders/basic.vs"),
                            utilities::read_file("assets/shaders/basic.fs"));
    cube_texture =
        utilities::texture_load_from_path(api, "assets/textures/dice.png");
  }

  void on_app_loop(float delta_seconds) override {
    auto &api = get_graphics_api();
    auto mvp = compute_mvp(delta_seconds);
    auto ortho = compute_mvp_ortho(delta_seconds);

    // Bind the cube program and configure it
    cube_shader->use();
    cube_shader->bind_textures({{"diffuse", cube_texture}});
    // The pre_draw hook should be called before any rendering operation occurs
    api->pre_draw();
    {
      auto *shader_data =
          api->map_constant_buffer<utilities::application_data>();
      shader_data->mvp = mvp;
      api->unmap_constant_buffer();
      cube_mesh->draw();
    }
    {
      auto *shader_data =
          api->map_constant_buffer<utilities::application_data>();
      shader_data->mvp = ortho;
      api->unmap_constant_buffer();
      quad_mesh->draw();
    }
    api->post_draw();

    m_rot_y += M_PI * 0.25f * delta_seconds;
  }
};

int main(int argc, char **argv) {
  cube_app app;
  return app.loop();
}