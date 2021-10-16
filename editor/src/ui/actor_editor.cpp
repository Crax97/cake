#include "ui/actor_editor.h"

#include "editor_application.h"
#include "game_framework/component.h"
#include "game_framework/components/component_repository.h"

#include "game_framework/actor.h"
#include "game_framework/components/sprite_component.h"
#include "game_framework/properties/property.h"
#include "game_framework/stage.h"
#include "glm/gtc/quaternion.hpp"
#include "graphics_api.h"

#include "game_framework/properties/property_visitor.h"

#include "imgui.h"
#include "texture.h"

void editor::actor_editor::draw_editor() noexcept {
  if (ImGui::Begin("Actor Editor")) {
    if (m_selected_actor.expired()) {
      ImGui::Text("No actor selected");
      ImGui::End();
      return;
    }
    show_actor_transform(m_selected_actor.lock());
    show_actor_components(m_selected_actor.lock());
    ImGui::End();
  }
}

void draw_property(property_system::property *prop,
                   spectacle::component *component) {
  class imgui_drawer : public property_system::property_visitor {
    void visit_int_property(int &value) {}
    void visit_double_property(double &value) {}
    void visit_float_property(float &value) {}
    void visit_string_property(std::string &value) {}
    void visit_vec2_property(glm::vec2 &value) {
      ImGui::DragFloat2("vec2", &value[0]);
    }
    void visit_vec3_property(glm::vec3 &value) {}
    void visit_vec4_property(glm::vec4 &value) {}
    void visit_texture_property(renderer::texture &value) {
      ImGui::Image(value.get_texture_object(), ImVec2(100, 100));
    }
  };

  auto drawer = imgui_drawer{};
  prop->visit(component, drawer);
}

void editor::actor_editor::show_actor_transform(
    std::shared_ptr<spectacle::actor> the_actor) noexcept {
  ImGui::Text("Actor Transform");
  bool needs_update = false;

  if (ImGui::DragFloat3("Location",
                        &the_actor->get_transform().get_location()[0])) {
    needs_update = true;
  }
  auto rotation_quat = the_actor->get_transform().get_rotation();
  auto euler_rotation = glm::eulerAngles(rotation_quat);
  if (ImGui::DragFloat3("Rotation", &euler_rotation[0])) {
    the_actor->set_rotation(glm::quat(euler_rotation));
  }

  if (ImGui::DragFloat3("Scale", &the_actor->get_transform().get_scale()[0])) {
    needs_update = true;
  }
  if (needs_update) {
    the_actor->get_transform().update_transform_matrix();
  }
}
void editor::actor_editor::show_actor_components(
    std::shared_ptr<spectacle::actor> the_actor) noexcept {
  the_actor->for_each_component([&](spectacle::component &component) {
    ImGui::Text("%s", component.get_component_name());
    ImGui::Indent(1);
    for (auto &property : component.get_editable_properties()) {
      draw_property(property.get(), &component);
    }
    ImGui::Indent(-1);
  });
}
void editor::actor_editor::select_actor(
    std::shared_ptr<spectacle::actor> new_actor) {
  m_selected_actor = new_actor;
}