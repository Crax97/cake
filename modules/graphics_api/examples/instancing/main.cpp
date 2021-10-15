
#include "Init.h"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_video.h"
#include "api_factory.h"
#include "texture.h"
#include "window.h"

#include "application.h"
#include "utilities.h"
#include <memory>

class instancing_app : public example_support_library::application {
private:
  struct instance_data {
    glm::vec3 offset;
  };

  float m_pitch = 0.0f;
  float m_yaw = 0.0f;
  glm::vec3 movement{0};
  glm::vec3 location{0};
  std::shared_ptr<renderer::mesh> quad_mesh;
  std::shared_ptr<renderer::instanced_mesh> instanced_quad;
  std::shared_ptr<renderer::texture> grass_texture;
  std::shared_ptr<renderer::shader> instancing_program;

  glm::mat4 compute_mvp(float delta_seconds) {
    auto model = glm::rotate(m_yaw, glm::vec3(0, 1, 0)) *
                 glm::rotate(m_pitch, glm::vec3(1, 0, 0));
    auto right = glm::vec3(model[0]);
    auto up = glm::vec3(model[1]);
    auto fwd = glm::vec3(model[2]);
    location += (fwd * movement.z + right * movement.x + up * movement.y) *
                delta_seconds * 20.0f;

    const float fov = glm::radians(45.0f);
    auto projection = glm::perspective(fov, 800.0f / 600.0f, 0.01f, 10000.0f);
    auto view = glm::lookAt(location, location + fwd, glm::vec3(0, 1, 0));
    auto mv = view;
    auto mvp = projection * mv;
    return mvp;
  }

  instance_data create_instance(int x, int z) {
    const glm::vec3 scale = glm::vec3(0.5f, 0.0f, 0.1f);
    const auto translation =
        glm::vec3(static_cast<float>(x), 0.0f, static_cast<float>(z)) * scale;
    return instance_data{translation};
  }

public:
  instancing_app()
      : example_support_library::application("Instancing example", 1920, 1080) {
  }
  void on_app_startup() override {
    class c_quad_data_descriptor : public renderer::instance_descriptor {

      std::vector<renderer::attrib_descriptor> get_descriptors() override {
        return {{
            "offset",
            renderer::attrib_data_type::Float,
            renderer::attrib_element_count::Three,
            offsetof(instance_data, offset),
        }};
      }
      size_t get_stride() override { return 0; }
      size_t get_element_size() override { return sizeof(instance_data); }
    };

    auto quad_data_descriptor = std::make_unique<c_quad_data_descriptor>();

    auto &api = get_graphics_api();
    quad_mesh = utilities::create_quad(api);
    instancing_program = api->compile_shader(
        utilities::read_file("assets/shaders/instancing.vs"),
        utilities::read_file("assets/shaders/instancing.fs"));
    grass_texture =
        utilities::texture_load_from_path(api, "assets/textures/grass.png");

    instanced_quad =
        api->create_instanced_mesh(quad_mesh, std::move(quad_data_descriptor));
    const auto rows = 10000;
    const auto columns = 50;
    const int r_b = -rows / 2;
    const int r_e = rows / 2;
    for (int z = r_b; z < r_e; z++) {
      const int c_b = -columns / 2;
      const int c_e = columns / 2;
      for (int x = c_b; x < c_e; x++) {
        auto instance = create_instance(x, z);
        instanced_quad->add_instance(instance);
      }
    }

    instanced_quad->configure_for_shader(instancing_program);
  }

  void on_app_loop(float delta_seconds) override {
    auto &api = get_graphics_api();
    auto mvp = compute_mvp(delta_seconds);
    instancing_program->set_uniform_matrix4x4("mvp", &mvp[0][0]);
    instancing_program->set_uniform_float("time", get_total_time());

    api->pre_draw();

    // draw automatically uses the shader for which it's configured
    // with instanced drawing
    instanced_quad->draw();
    api->post_draw();

    float fps = 60.0f / delta_seconds;
    std::string title = "Instancing FPS: " + std::to_string(fps);
    set_title(title);
  }
  void on_app_keydown(const SDL_KeyboardEvent &event) override {
    if (event.keysym.sym == SDLK_a) {
      movement.x = 1.0f;
    } else if (event.keysym.sym == SDLK_d) {
      movement.x = -1.0f;
    }
    if (event.keysym.sym == SDLK_w) {
      movement.z = 1.0f;
    } else if (event.keysym.sym == SDLK_s) {
      movement.z = -1.0f;
    }
    if (event.keysym.sym == SDLK_q) {
      movement.y = 1.0f;
    } else if (event.keysym.sym == SDLK_e) {
      movement.y = -1.0f;
    }

    if (event.keysym.sym == SDLK_F8) {
      get_window()->set_show_cursor(!get_window()->get_show_cursor());
    }
  }
  void on_app_keyup(const SDL_KeyboardEvent &event) override {

    if (event.keysym.sym == SDLK_a || event.keysym.sym == SDLK_d) {
      movement.x = 0.0f;
    }
    if (event.keysym.sym == SDLK_w || event.keysym.sym == SDLK_s) {
      movement.z = 0.0f;
    }
    if (event.keysym.sym == SDLK_q || event.keysym.sym == SDLK_e) {
      movement.y = 0.0f;
    }
  }
  void on_app_mousemove(const int delta_x, const int delta_y) override {
    if (get_window()->get_show_cursor())
      return;
    const float mouse_cam_speed = 3.14f;
    auto delta = get_window()->window_to_relative(delta_x, delta_y);
    m_pitch = glm::clamp(m_pitch - delta.y * get_delta_time() * mouse_cam_speed,
                         -3.14f * 0.5f, 3.14f * 0.5f);
    m_yaw -= delta.x * get_delta_time() * mouse_cam_speed;
  }
};

int main(int argc, char **argv) {
  instancing_app app;
  return app.loop();
}