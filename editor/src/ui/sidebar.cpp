#include "ui/sidebar.h"
#include "editor_application.h"
#include "game_framework/components/component_repository.h"

#include "actor.h"
#include "stage.h"

#include "imgui.h"

void editor::sidebar::draw_sidebar() noexcept {
  if (ImGui::Begin("Stage Outline", nullptr)) {
    m_editor.get_editing_stage()->for_each_actor(
        [&](std::shared_ptr<spectacle::actor> m_actor) {
          ImGui::PushID("actor");
          ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf;
          bool is_selected = m_selected_actor == m_actor;
          if (is_selected) {
            node_flags |= ImGuiTreeNodeFlags_Selected;
          }

          if (ImGui::TreeNodeEx("actor", node_flags)) {
            if (ImGui::IsItemClicked() && !is_selected) {
              m_editor.on_actor_selected(m_actor);
              m_selected_actor = m_actor;
            }
            ImGui::TreePop();
          }

          ImGui::PopID();
        });

    if (ImGui::BeginPopupContextItem("sidebar")) {
      ImGui::Text("%s", "Actions");

      if (ImGui::Button("Create new Actor")) {
        auto actor = std::make_shared<spectacle::actor>();
        m_editor.get_editing_stage()->add_new_actor(actor);
      }
      ImGui::EndPopup();
    }
    ImGui::End();
  }
}