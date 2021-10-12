#include "editor_application.h"

#include "SDL_video.h"
#include "actor.h"
#include "game_framework/components/component_repository.h"
#include "game_framework/components/sprite_component.h"
#include "glad/glad.h"
#include "graphics_api.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "initialization_object.h"
#include "input/inputsystem.h"
#include "input/keys.h"
#include "mesh.h"
#include "stage.h"
#include "texture.h"
#include "window.h"

#include <iostream>
#include <memory>

editor_application::editor_application(int argc, char **argv)
    : gameframework::game(argc, argv) {}

void editor_application::on_new_level() noexcept {
  if (m_level_is_dirty) {
    // notify level is dirty
  }

  m_stage->on_stage_exit();
  m_stage = std::make_unique<spectacle::stage>();
  m_level_is_dirty = false;
}
void editor_application::on_load_request() noexcept {}
void editor_application::on_save_request() noexcept {}

void editor_application::draw_menubar() noexcept {
  if (ImGui::BeginMainMenuBar()) {
    menubar_height = ImGui::GetWindowSize().y;

    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New")) {
        on_new_level();
      }
      if (ImGui::MenuItem("Load")) {
        on_load_request();
      }
      if (ImGui::MenuItem("Save")) {
        on_save_request();
      }
      ImGui::EndMenu();
    }

    ImGui::MenuItem("Edit");
    ImGui::MenuItem("Help");
    ImGui::EndMainMenuBar();
  }
}

void editor_application::draw_sidebar() noexcept {
  ImGui::SetNextWindowSize(ImVec2(sidebar_width, get_window().GetHeight()));
  ImGui::SetNextWindowPos({0, static_cast<float>(menubar_height)});
  if (ImGui::Begin("sidebar", nullptr,
                   ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
    ImGui::Text("%s", "Actors");
    m_stage->for_each_actor([&](std::shared_ptr<spectacle::actor> m_actor) {
      ImGui::PushID("actor");
      ImGui::Text("actor");
      // select_actor(m_actor);
      if (ImGui::BeginPopupContextItem("actor",
                                       ImGuiPopupFlags_MouseButtonLeft)) {
        ImGui::Text("%s", "Actor Actions");
        auto component_names =
            gameframework::component_repository::the().get_component_names();
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
        m_stage->add_new_actor(actor);
      }
      ImGui::EndPopup();
    }
    ImGui::End();
  }
}

void editor_application::update_imgui() noexcept {
  ImGui::SetCurrentContext(m_imgui_context);
  glViewport(0, 0, get_window().GetWidth(), get_window().GetHeight());

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  if (m_show_demo) {
    ImGui::ShowDemoWindow(&m_show_demo);
  }
  draw_menubar();
  draw_sidebar();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void editor_application::on_app_update() noexcept {
  game::on_app_update();
  if (inputsystem::inputsystem::is_button_just_pressed(
          inputsystem::keyboard_key::F8)) {
    m_show_demo = !m_show_demo;
  }
  update_imgui();
}

void editor_application::on_app_startup() noexcept {
  game::on_app_startup();

  m_imgui_context = ImGui::CreateContext();
  auto ctx = SDL_GL_GetCurrentContext();
  auto *win = SDL_GL_GetCurrentWindow();
  const char *version = "#version 460 core";
  ImGui_ImplSDL2_InitForOpenGL(win, ctx);
  ImGui_ImplOpenGL3_Init(version);

  // TEST TEST REMOVE ME
  // auto actor = std::make_shared<spectacle::actor>();
  // auto sprite_component =
  // std::make_shared<gameframework::sprite_component>(); auto init_object =
  // spectacle::initialization_object{}; init_object.add("texture",
  // "assets/textures/grass.png"); sprite_component->initialize(init_object);
  // actor->add_new_component(sprite_component);
}

void editor_application::on_app_shutdown() noexcept {
  game::on_app_shutdown();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

editor_application::~editor_application() noexcept {}