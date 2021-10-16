#include "ui/sidebar.h"
#include "editor_application.h"
#include "game_framework/components/component_repository.h"

#include "actor.h"
#include "stage.h"

#include "imgui.h"

void editor::sidebar::draw_sidebar() noexcept {
  if (ImGui::Begin("Stage Outline", nullptr)) {
    ImGui::Text("%s", "Actors");
    m_editor.get_editing_stage()->for_each_actor(
        [&](std::shared_ptr<spectacle::actor> m_actor) {
          ImGui::PushID("actor");
          ImGui::Text("actor");
          // select_actor(m_actor);
          if (ImGui::BeginPopupContextItem("actor",
                                           ImGuiPopupFlags_MouseButtonLeft)) {
            ImGui::Text("%s", "Actor Actions");
            auto component_names = gameframework::component_repository::the()
                                       .get_component_names();
            for (auto &component : component_names) {
              if (ImGui::Button(component.c_str())) {
                // add new component to selected actor
              }
            }
            ImGui::EndPopup();
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