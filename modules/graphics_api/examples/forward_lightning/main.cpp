
#include "Init.h"
#include "api_factory.h"
#include "texture.h"
#include "window.h"

#include "application.h"
#include "utilities.h"
#include <memory>

class lighting_app : public example_support_library::application {
private:
  float m_rot_y = 0.0f;
  std::shared_ptr<renderer::mesh> cube_mesh;
  std::shared_ptr<renderer::mesh> quad_mesh;
  std::shared_ptr<renderer::texture> cube_texture;
  std::shared_ptr<renderer::shader> cube_shader;

  glm::mat4 compute_mvp_cube(float delta_seconds) {
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
  glm::mat4 compute_mvp_plane(float delta_seconds) {
    auto &api = get_graphics_api();

    auto projection = glm::perspective(static_cast<float>(M_PI) * 0.7f,
                                       800.0f / 600.0f, 0.01f, 10000.0f);
    auto view = glm::lookAt(glm::vec3(0.0f, 30.0f, -30.0f), glm::vec3(0.0f),
                            glm::vec3(0, 1, 0));
    auto model = glm::translate(glm::vec3(0.0f, -10.0f, 0.0f)) *
                 glm::scale(glm::vec3(500.0f)) *
                 glm::rotate(m_rot_y, glm::vec3(0, 1, 0)) *
                 glm::rotate(-static_cast<float>(M_PI) * 0.5f,
                             glm::vec3(1.0f, 0.0f, 0.0f));
    auto mv = view * model;
    auto mvp = projection * mv;
    return mvp;
  }

public:
  lighting_app()
      : example_support_library::application("Lightning example", 800, 600) {}
  void on_app_startup() override {

    auto &api = get_graphics_api();
    cube_mesh =
        utilities::mesh_load_from_path(api, "assets/models/cube.obj")[0];
    quad_mesh = utilities::create_quad(api);
    cube_shader = api->compile_shader(
        utilities::read_file("assets/shaders/basic_no_ub.vs"),
        utilities::read_file("assets/shaders/basic.fs"));
    cube_texture =
        utilities::texture_load_from_path(api, "assets/textures/dice.png");
  }

  void on_app_loop(float delta_seconds) override {
    auto &api = get_graphics_api();
    auto mvp_cube = compute_mvp_cube(delta_seconds);
    auto mvp_plane = compute_mvp_plane(delta_seconds);

    api->pre_draw();
    // 1. Draw from the light's perspective
    /*
    {
      light_framebuffer->bind();
      light_shader->use();
      auto light_mv = compute_light_mv(delta_seconds);
      draw_scene(api, light_mv, cube_model, plane_model);
      light_framebuffer->unbind();
    }
    */
    // 2. Draw scene normally, using the light framebuffer
    //    to shade the scene.
    /*
    {
      lit_shader->use();
      auto camera_mv = compute_camera_mv(delta_seconds);
      draw_scene(api, camera_mv, cube_model, plane_model);
    }
    */
    // Bind the cube program and configure it
    cube_shader->use();
    cube_shader->bind_textures({{"diffuse", cube_texture}});
    // The pre_draw hook should be called before any rendering operation occurs
    cube_shader->set_uniform_matrix4x4("mvp", &mvp_cube[0][0]);
    cube_mesh->draw();

    cube_shader->set_uniform_matrix4x4("mvp", &mvp_plane[0][0]);
    quad_mesh->draw();

    api->post_draw();

    m_rot_y += M_PI * 0.25f * delta_seconds;
  }
};

int main(int argc, char **argv) {
  lighting_app app;
  return app.loop();
}