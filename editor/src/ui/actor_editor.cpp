#include "ui/actor_editor.h"

#include <utility>
#include <string_view>

#include "editor_application.h"
#include "game_framework/component.h"
#include "game_framework/components/component_repository.h"
#include "game_framework/resources/texture_resource.h"

#include "game_framework/actor.h"
#include "game_framework/stage.h"
#include "glm/gtc/quaternion.hpp"
#include "graphics_api.h"

#include "object/fields.h"

#include "ui/dialogs.h"

#include "imgui.h"
#include "imgui_stdlib.h"
#include "texture.h"

void editor::actor_editor::draw_editor() noexcept {
    ImGui::PushID(this);

    if (m_selected_actor.expired()) {
      ImGui::Text("No actor selected");
      ImGui::PopID();
      return;
    }
    show_actor_fields(m_selected_actor.lock());
    show_actor_components(m_selected_actor.lock());
    if(wants_to_add_component && ImGui::Begin("Add new component", &wants_to_add_component, ImGuiWindowFlags_Modal)) {
        for (auto &name: gameframework::component_repository::the().get_component_names()) {
            if (ImGui::Button(name.c_str())) {
                auto new_component = gameframework::component_repository::the().construct_component(name,
                                                                                                    *m_selected_actor.lock());
                m_selected_actor.lock()->add_new_component(new_component.value());
                wants_to_add_component = false;
            }
        }
        ImGui::End();
    }

    ImGui::PopID();
}

void draw_property(field *prop,
                   const std::weak_ptr<spectacle::component>& component) {
      class imgui_drawer : public field_visitor {
      public:
            field *m_prop;
            std::weak_ptr<spectacle::component>  m_comp;
            explicit imgui_drawer(field *in_prop, std::weak_ptr<spectacle::component> in_comp) : m_prop(in_prop), m_comp(std::move(in_comp)) {}

            void visit_bool_property(const bool &value) override {
                auto copy = value;
                if(ImGui::Checkbox(m_prop->get_name().c_str(), &copy)) {
                    m_prop->set(m_comp.lock().get(), copy);
                }
            }
            void visit_int_property(const int &value) override {
                auto copy = value;
                if(ImGui::InputInt(m_prop->get_name().c_str(), &copy)) {
                    m_prop->set(m_comp.lock().get(), copy);
                }
            }
            void visit_double_property(const double &value) override {
                auto copy = value;
                if(ImGui::InputDouble(m_prop->get_name().c_str(), &copy)) {
                    m_prop->set(m_comp.lock().get(), copy);
                }
            }
            void visit_float_property(const float &value) override {
                auto copy = value;
                if(ImGui::InputFloat(m_prop->get_name().c_str(), &copy)) {
                    m_prop->set(m_comp.lock().get(), copy);
                }
            }
            void visit_string_property(const std::string &value) override {
                std::string copy = value;
                if(ImGui::InputText(m_prop->get_name().c_str(), &copy, ImGuiInputTextFlags_None)) {
                    m_prop->set(m_comp.lock().get(), copy);
                }
            }
            void visit_vec2_property(const glm::vec2 &value) override {
                auto copy = value;
                if(ImGui::DragFloat2(m_prop->get_name().c_str(), &copy[0])) {
                    m_prop->set(m_comp.lock().get(), copy);
                }
            }
            void visit_vec3_property(const glm::vec3 &value) override {
                auto copy = value;
                if(ImGui::DragFloat3(m_prop->get_name().c_str(), &copy[0])) {
                    m_prop->set(m_comp.lock().get(), copy);
                }
            }
            void visit_vec4_property(const glm::vec4 &value) override {
                auto copy = value;
                if(ImGui::DragFloat4(m_prop->get_name().c_str(), &copy[0])) {
                    m_prop->set(m_comp.lock().get(), copy);
                }
            }
            void visit_char_property(const char &value) override {

            }
            void visit_quat_property(const glm::quat& q) override {

            }
            void visit_resource_property(const class resource& res) override {
                if(res.is_resource<game_framework::texture_resource>()) {
                    auto texture_res = res.get<game_framework::texture_resource>();
                    if(texture_res) {
                        ImGui::Image(texture_res.get_texture()->get_texture_object(), ImVec2(100, 100));
                    } else {
                        ImGui::Text("No valid texture");
                    }
                }
                ImGui::SameLine();
                if(ImGui::Button("Change Resource"))  {
                    auto resource_path = editor::dialogs::file_picker("Pick Resource", {"*.png", "*.jpg"}, ".", "Texture Resource", false);
                    if(resource_path) {
                        m_prop->set_from_string(m_comp.lock().get(), resource_path->string());
                    }
                }
            }

            void visit_pointer_property(const pointer_field_base& ptr) override {
                if(ptr.points_to<renderer::texture>()) {
                    auto texture = ptr.get_ptr<renderer::texture>(m_comp.lock().get());
                    if(texture && texture->has_valid_texture()) {
                        ImGui::Image(texture->get_texture_object(), ImVec2(512, 512));
                    } else {
                        ImGui::Text("No valid texture");
                    }
                }
            }
            void visit_container_property(const container_field& container) override {
                std::cout << "container" << "\n";
            }
            // void visit_texture_property(renderer::texture &value) {
              // ImGui::Text("%s", m_prop->get_property_name().c_str());
              // ImGui::SameLine();
              // ImGui::Image(value.get_texture_object(), ImVec2(100, 100));
    };

    auto drawer = imgui_drawer{prop, component};
    prop->visit(component.lock().get(), drawer);
}

void editor::actor_editor::show_actor_fields(
    const std::shared_ptr<spectacle::actor>& the_actor) noexcept {
    ImGui::Text("Editing %s", the_actor->get_name().data());
    ImGui::SameLine();
    if(ImGui::Button("+")) {
        wants_to_add_component = true;
    }
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
  the_actor->for_each_component([&](std::shared_ptr<spectacle::component> &component) {
    ImGui::Text("%s", component->get_descriptor()->get_name().c_str());
    ImGui::SameLine();
    if(ImGui::Button("-")) {
        component->mark_for_removal();
        return;
    }
    ImGui::Indent(1);
    for (auto &field : component->get_descriptor()->get_fields()) {
      draw_property(field.get(), component);
    }
    ImGui::Indent(-1);
  });
}
void editor::actor_editor::select_actor(
    std::shared_ptr<spectacle::actor> new_actor) {
  m_selected_actor = new_actor;
}