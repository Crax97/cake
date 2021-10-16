#include "ui/actor_editor.h"

#include "editor_application.h"
#include "game_framework/components/component_repository.h"

#include "actor.h"
#include "stage.h"

#include "imgui.h"

void editor::actor_editor::draw_editor() noexcept {
  if (ImGui::Begin("Actor Editor")) {
    if (m_selected_actor.expired()) {
      ImGui::Text("No actor selected");
      ImGui::End();
      return;
    }

    ImGui::End();
  }
}

void editor::actor_editor::select_actor(
    std::shared_ptr<spectacle::actor> new_actor) {
  m_selected_actor = new_actor;
}