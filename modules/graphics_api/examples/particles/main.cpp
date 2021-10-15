
#include "Init.h"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_video.h"
#include "api_factory.h"
#include "framebuffer.h"
#include "texture.h"
#include "window.h"

#include "application.h"
#include "utilities.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <memory>
#include <random>
#include <unordered_map>

using namespace std::literals;

class particles_app : public example_support_library::application {
private:
  struct instance_data {
    glm::vec3 initial_velocity;
    glm::vec3 color;
    float opacity;
    float lifetime;
    float initial_rotation;
    float rotation_rate;
  };
  constexpr static int max_instances = 25000;
  constexpr static float max_lifetime = 5.0f; // seconds
  std::default_random_engine m_engine;
  glm::vec3 movement{0};
  glm::vec3 location{0, 0, 0};

  std::vector<instance_data> particles;
  std::shared_ptr<renderer::mesh> quad_mesh;
  std::shared_ptr<renderer::instanced_mesh> instanced_quad;
  std::shared_ptr<renderer::texture> particles_texture;
  std::shared_ptr<renderer::shader> particles_program;
  std::shared_ptr<renderer::shader> simple_program;
  std::shared_ptr<renderer::framebuffer> framebuffer;

  glm::mat4 compute_mvp(float delta_seconds) {
    location += movement * delta_seconds * 20.0f;
    auto translation = glm::translate(location);
    auto model = translation * glm::scale(glm::vec3(10.0f));
    auto projection = glm::ortho(400.0f, -400.0f, -300.0f, 300.0f);
    auto mvp = projection * model;
    return mvp;
  }

  template <typename T>
  T random_interval(T a = std::numeric_limits<T>::min(),
                    T b = std::numeric_limits<T>::max()) {
    std::uniform_real_distribution<T> distrib(a, b);
    return distrib(m_engine);
  }

  glm::vec3 make_random_initial_velocity() {
    float x = random_interval(-40.0f, 40.0f);
    float y = random_interval(6.0f, 18.0f);
    float z = 0.0f;
    return {x, y, z};
  }
  glm::vec3 make_random_color() {
    float x = random_interval(0.0f, 1.0f);
    float y = random_interval(0.0f, 1.0f);
    float z = random_interval(0.0f, 1.0f);

    return {x, y, z};
  }

  void setup_instance(instance_data &instance) {
    instance.initial_velocity = make_random_initial_velocity();
    instance.lifetime = random_interval(0.0f, 3.5f);
    instance.initial_rotation = random_interval(0.0f, 3.14f);
    instance.rotation_rate = random_interval(-3.14f, 3.14f);
    instance.color = make_random_color();
    instance.opacity = random_interval(0.0f, 1.0f);
  };
  void update_instances(float delta_time) {
    for (auto &instance_data : particles) {
      instance_data.lifetime += delta_time;
      if (instance_data.lifetime >= max_lifetime) {
        setup_instance(instance_data);
      } else {
        /*
        instance_data.rotation += delta_time;
        instance_data.rotation_mat =
            glm::mat3(glm::rotate(instance_data.rotation, glm::vec3(0, 0, 1)));
            */
      }
    }
  }
  void copy_instance_buffer() {
    instance_data *data_ptr =
        instanced_quad->map_instance_buffer<instance_data>(max_instances);
    std::memcpy(data_ptr, particles.data(),
                particles.size() * sizeof(instance_data));
    instanced_quad->unmap_instance_buffer();
  }

public:
  particles_app()
      : example_support_library::application("Particles example", 1920, 1080) {}
  void on_app_startup() override {
    class c_quad_data_descriptor : public renderer::instance_descriptor {

      std::vector<renderer::attrib_descriptor> get_descriptors() override {
        return {
            {
                "initial_velocity",
                renderer::attrib_data_type::Float,
                renderer::attrib_element_count::Three,
                offsetof(instance_data, initial_velocity),
            },
            {
                "initial_rotation",
                renderer::attrib_data_type::Float,
                renderer::attrib_element_count::Mat3,
                offsetof(instance_data, initial_rotation),
            },
            {
                "rotation_rate",
                renderer::attrib_data_type::Float,
                renderer::attrib_element_count::Mat3,
                offsetof(instance_data, rotation_rate),
            },
            {
                "color",
                renderer::attrib_data_type::Float,
                renderer::attrib_element_count::Three,
                offsetof(instance_data, color),
            },
            {
                "opacity",
                renderer::attrib_data_type::Float,
                renderer::attrib_element_count::One,
                offsetof(instance_data, opacity),
            },
            {
                "lifetime",
                renderer::attrib_data_type::Float,
                renderer::attrib_element_count::One,
                offsetof(instance_data, lifetime),
            },
        };
      }
      size_t get_stride() override { return sizeof(instance_data); }
      size_t get_element_size() override { return sizeof(instance_data); }
    };

    auto quad_data_descriptor = std::make_unique<c_quad_data_descriptor>();

    auto &api = get_graphics_api();
    quad_mesh = utilities::create_quad(api);
    particles_program = api->compile_shader(
        utilities::read_file("assets/shaders/particles.vs"),
        utilities::read_file("assets/shaders/particles.fs"));
    simple_program = api->compile_shader(
        utilities::read_file("assets/shaders/simple.vs"),
        utilities::read_file("assets/shaders/post_render_effects/rainbow.fs"));
    particles_texture =
        utilities::texture_load_from_path(api, "assets/textures/particle.png");

    instanced_quad =
        api->create_instanced_mesh(quad_mesh, std::move(quad_data_descriptor));
    particles.resize(max_instances);
    std::for_each(particles.begin(), particles.end(),
                  [&](auto &particle) { setup_instance(particle); });
    instanced_quad->configure_for_shader(particles_program);
    framebuffer = api->create_framebuffer(1024, 720);
  }

  void on_app_loop(float delta_seconds) override {
    auto &api = get_graphics_api();
    auto mvp = compute_mvp(delta_seconds);
    auto gravity = glm::vec3(0.0f, -9.8f, 0.0f);
    particles_program->use();
    particles_program->bind_textures({{"diffuse"s, particles_texture}});
    particles_program->set_uniform_matrix4x4("mvp", &mvp[0][0]);
    particles_program->set_uniform_float("time", get_total_time());
    simple_program->set_uniform_float("time", get_total_time());
    particles_program->set_uniform_vec3("gravity", &gravity.x);
    update_instances(delta_seconds);
    copy_instance_buffer();

    api->pre_draw();
    framebuffer->bind();
    // draw automatically uses the shader for which it's configured
    // with instanced drawing
    instanced_quad->draw();
    framebuffer->unbind();

    simple_program->use();
    simple_program->bind_textures(
        {{"diffuse"s, framebuffer->get_color_texture()}});
    quad_mesh->draw();

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
    if (event.keysym.sym == SDLK_q || event.keysym.sym == SDLK_e) {
      movement.y = 0.0f;
    }
  }
};

int main(int argc, char **argv) {
  particles_app app;
  return app.loop();
}