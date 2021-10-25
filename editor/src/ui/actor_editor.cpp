#include "ui/actor_editor.h"

#include "editor_application.h"
#include "game_framework/component.h"
#include "game_framework/components/component_repository.h"

#include "game_framework/actor.h"
#include "game_framework/stage.h"
#include "glm/gtc/quaternion.hpp"
#include "graphics_api.h"

#include "object/field_visitor.h"
#include "object/field.h"
#include "object/pointer_field.h"

#include "imgui.h"
#include "texture.h"

void editor::actor_editor::draw_editor() noexcept {
  if (ImGui::Begin("Actor Editor")) {
    if (m_selected_actor.expired()) {
      ImGui::Text("No actor selected");
      ImGui::End();
      return;
    }
    show_actor_fields(m_selected_actor.lock());
    show_actor_components(m_selected_actor.lock());
    ImGui::End();
  }
}

void draw_property(field *prop,
                   spectacle::component *component) {
      class imgui_drawer : public field_visitor {
      public:
            field *m_prop;
            spectacle::component* m_comp;
            explicit imgui_drawer(field *in_prop, spectacle::component* in_comp) : m_prop(in_prop), m_comp(in_comp) {}

            void visit_bool_property(bool &value) override {
                ImGui::Checkbox(m_prop->get_name().c_str(), &value);
            }
            void visit_int_property(int &value) override {
                ImGui::InputInt(m_prop->get_name().c_str(), &value);
            }
            void visit_double_property(double &value) override {
                ImGui::InputDouble(m_prop->get_name().c_str(), &value);
            }
            void visit_float_property(float &value) override {
                ImGui::InputFloat(m_prop->get_name().c_str(), &value);
            }
            void visit_string_property(std::string &value) override {}
            void visit_vec2_property(glm::vec2 &value) override {
                ImGui::DragFloat2(m_prop->get_name().c_str(), &value[0]);
            }
            void visit_vec3_property(glm::vec3 &value) override {
                ImGui::DragFloat3(m_prop->get_name().c_str(), &value[0]);}
            void visit_vec4_property(glm::vec4 &value) override {
                ImGui::DragFloat4(m_prop->get_name().c_str(), &value[0]);}
            void visit_char_property(char &value) override {

            }
            void visit_quat_property(glm::quat& q) override {

            }

            void visit_pointer_property(pointer_field& ptr) override {
                if(ptr.points_to<renderer::texture>()) {
                    auto texture = ptr.get_ptr<renderer::texture>(m_comp);
                    if(texture && texture->has_valid_texture()) {
                        ImGui::Image(texture->get_texture_object(), ImVec2(512, 512));
                    } else {
                        ImGui::Text("No valid texture");
                    }
                }
            }
            // void visit_texture_property(renderer::texture &value) {
              // ImGui::Text("%s", m_prop->get_property_name().c_str());
              // ImGui::SameLine();
              // ImGui::Image(value.get_texture_object(), ImVec2(100, 100));
    };

    auto drawer = imgui_drawer{prop, component};
    prop->visit(component, drawer);
}

void editor::actor_editor::show_actor_fields(
    const std::shared_ptr<spectacle::actor>& the_actor) noexcept {
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
    ImGui::Text("%s", component.get_descriptor()->get_name().c_str());
    ImGui::Indent(1);
    for (auto &field : component.get_descriptor()->get_fields()) {
      draw_property(field.get(), &component);
    }
    ImGui::Indent(-1);
  });
}
void editor::actor_editor::select_actor(
    std::shared_ptr<spectacle::actor> new_actor) {
  m_selected_actor = new_actor;
}