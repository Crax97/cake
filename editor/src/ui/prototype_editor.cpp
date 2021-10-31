//
// Created by crax on 10/31/21.
//

#include "ui/prototype_editor.h"

#include "game_framework/loaders/actor_loader.h"
#include "game_framework/actor.h"

#include <imgui.h>

namespace editor {
    void prototype_editor::update() {
        if (m_is_open && ImGui::Begin("Prototypes", &m_is_open)) {
            if(ImGui::BeginTable("PrototypeEditorTable", 2)) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                for (auto &prototype: gameframework::actor_loader::the().get_actors()) {
                    if (ImGui::Button(prototype->get_name().data())) {
                        m_actor_editor.select_actor(prototype);
                        m_selected_actor = prototype;
                    }
                }

                ImGui::TableSetColumnIndex(1);
                m_actor_editor.draw_editor();
                ImGui::EndTable();
            }
            ImGui::End();

        }
    }
}