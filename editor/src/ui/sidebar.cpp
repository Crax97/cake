#include "ui/sidebar.h"
#include "editor_application.h"
#include "game_framework/components/component_repository.h"
#include "game_framework/loaders/actor_loader.h"
#include "ui/dialogs.h"

#include "game_framework/actor.h"
#include "game_framework/stage.h"

#include "imgui.h"

void editor::sidebar::draw_sidebar() noexcept {
  if (ImGui::Begin("Stage Outline", nullptr)) {
      if(ImGui::Button("+")) {
          auto actor = std::make_shared<spectacle::actor>();
          m_editor.get_editing_stage()->add_new_actor(actor);
      }
    m_editor.get_editing_stage()->for_each_actor(
        [&](std::shared_ptr<spectacle::actor> m_actor) {
          ImGui::PushID(m_actor->get_name().data());
          ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf;
          bool is_selected = m_selected_actor == m_actor;
          if (is_selected) {
            node_flags |= ImGuiTreeNodeFlags_Selected;
          }

          if (ImGui::TreeNodeEx(m_actor->get_name().data(), node_flags)) {
            if (ImGui::IsItemClicked() && !is_selected) {
              m_editor.on_actor_selected(m_actor);
              m_selected_actor = m_actor;
              m_is_editing_actor = false;
            }
            if(is_selected && ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                m_is_editing_actor = true;

            }
            ImGui::TreePop();
          }
            if(m_is_editing_actor && ImGui::BeginPopupContextWindow("Properties")) {
                if(ImGui::Button("Change actor name")) {
                    auto new_name = dialogs::text_input("New actor name?", "Input new actor name", m_selected_actor->get_name().data());
                    if(new_name) {
                        m_selected_actor->set_name(new_name.value());
                    }
                    m_is_editing_actor = false;
                }
                if(ImGui::Button("Duplicate actor")) {
                    auto new_actor = m_selected_actor->clone();
                    m_editor.get_editing_stage()->add_new_actor(new_actor);
                    m_is_editing_actor = false;
                }
                if(ImGui::Button("Delete actor")) {
                    m_selected_actor->destroy();
                    m_selected_actor = nullptr;
                    m_editor.on_actor_selected(nullptr);
                    m_is_editing_actor = false;
                }
                if(ImGui::Button("Enable as prototype")) {
                    m_selected_actor->enable_prototype();
                    gameframework::actor_loader::the().add_prototype(m_selected_actor);
                    m_is_editing_actor = false;
                }
                ImGui::EndPopup();
            }
          ImGui::PopID();
        });
    ImGui::End();
  }
}