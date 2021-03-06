#include "editor_application.h"

#include "SDL_video.h"
#include "framebuffer.h"
#include "game_framework/actor.h"
#include "game_framework/components/component_repository.h"
#include "game_framework/components/sprite_component.h"
#include "game_framework/initialization_object.h"
#include "game_framework/stage.h"
#include "serializers/string_serializer.h"
#include "game_framework/loaders/stage_serializer.h"
#include "game_framework/loaders/actor_loader.h"
#include "game_framework/loaders/stage_deserializer.h"

#include "ui/dialogs.h"

#include "glad/glad.h"
#include "graphics_api.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "input/inputsystem.h"
#include "input/keys.h"
#include "mesh.h"
#include "window.h"
#include "object/function.h"

#include <iostream>
#include <memory>
#include <string>
#include <fstream>

using namespace std::string_literals;

editor_application::editor_application(int argc, char **argv)
    : gameframework::game(argc, argv), m_sidebar(*this) {
}

void editor_application::on_new_level() noexcept {
  if (m_level_is_dirty) {
    // notify level is dirty
  }

  m_stage->on_stage_exit();
  m_stage = std::make_unique<spectacle::stage>();
  m_level_is_dirty = false;
}

void editor_application::on_load_request() noexcept {
    auto level_load_path = editor::dialogs::file_picker("Open stage", {"*.stage"}, ".", "Miku stage file", false);
    if(!level_load_path) {
        return;
    }
    std::fstream input_level;
    input_level.open(level_load_path.value().c_str(), std::ios::in);
    if(input_level.bad()) {
        // Log level file opening errored
        return;
    }
    auto deserializer = gameframework::stage_deserializer();
    deserializer.deserialize(input_level);
    if(deserializer.good()) {
        on_new_level();
        m_stage = std::move(deserializer.get_stage());
    }
}
void editor_application::on_save_request() noexcept {
    auto level_save_path = editor::dialogs::save_file("Save stage", {".stage"}, "./level.stage", "Miku stage file");
    if(!level_save_path) {
        return;
    }

    std::fstream output_level;
    output_level.open(level_save_path->c_str(), std::ios::out | std::ios::trunc);
    gameframework::stage_serializer serializer(output_level);
    serializer.begin();
    m_stage->serialize(serializer);
    serializer.end();

    gameframework::actor_loader::the().dump_actors(".");
}

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

    if (ImGui::BeginMenu("Edit")) {
        if(ImGui::MenuItem("Edit Actor Prototypes")) {
            open_actor_prototype_editor();
        }
        ImGui::EndMenu();
    }
    ImGui::MenuItem("Help");
    ImGui::EndMainMenuBar();
  }
}

void editor_application::draw_sidebar() noexcept { m_sidebar.draw_sidebar(); }

void editor_application::draw_game_window() noexcept {
  auto render_texture = m_framebuffer->get_color_texture();
  ImGui::SetNextWindowSizeConstraints(ImVec2(800, 600),
                                      ImVec2(FLT_MAX, FLT_MAX));
  if (ImGui::Begin("Render window")) {
    auto *obj = render_texture->get_texture_object();
    ImGui::Image(obj, ImGui::GetWindowSize(), ImVec2(1, 0), ImVec2(0, -1));
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

  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

  // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window
  // not dockable into, because it would be confusing to have two docking
  // targets within each others.
  ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  const ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  window_flags |=
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

  ImGui::Begin("MikuEngine Editor Dockspace", nullptr, window_flags);
  ImGui::PopStyleVar(2);

  ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
  ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

  draw_game_window();
  draw_menubar();
  draw_sidebar();
  m_proto_editor.update();

    if (ImGui::Begin("Actor Editor")) {
        m_actor_editor.draw_editor();
        ImGui::End();
    }
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void editor_application::on_actor_selected(
    std::shared_ptr<spectacle::actor> actor) noexcept {
  m_actor_editor.select_actor(actor);
}

void editor_application::on_app_update() noexcept {
  game::on_app_update();
  if (inputsystem::inputsystem::is_button_just_pressed(
          inputsystem::keyboard_key::F8)) {
    m_show_demo = !m_show_demo;
  }
if (inputsystem::inputsystem::is_button_just_pressed(
        inputsystem::keyboard_key::F4)) {
    auto selected_actor = m_actor_editor.get_selected_actor();
    if (selected_actor) {
        string_serializer ser;
        selected_actor->serialize(ser);
        std::cout << ser.get_str();
    }
}
if (inputsystem::inputsystem::is_button_just_pressed(
        inputsystem::keyboard_key::F5)) {
    string_serializer ser;
    m_stage->serialize(ser);
    std::cout << ser.get_str();
}
}

void editor_application::render_world() noexcept {
  m_framebuffer->bind();
  game::render_world();
  m_framebuffer->unbind();
  update_imgui();
}

void editor_application::on_app_startup() noexcept {
  game::on_app_startup();
  m_framebuffer = get_api().create_framebuffer(1240, 720);

  m_imgui_context = ImGui::CreateContext();
  auto ctx = SDL_GL_GetCurrentContext();
  auto *win = SDL_GL_GetCurrentWindow();
  const char *version = "#version 460 core";
  ImGui_ImplSDL2_InitForOpenGL(win, ctx);
  ImGui_ImplOpenGL3_Init(version);

  auto &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // TEST TEST REMOVE ME
  auto actor = std::make_shared<spectacle::actor>();
  auto sprite_component =
      std::make_shared<gameframework::sprite_component>(*actor.get());
  auto init_object = spectacle::initialization_object{};
  init_object.add("texture_path"s, "./grass.png"s);
  sprite_component->initialize(init_object);
  actor->add_new_component(sprite_component);
  m_stage->add_new_actor(actor);
}

void editor_application::on_app_shutdown() noexcept {
  game::on_app_shutdown();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

editor_application::~editor_application() noexcept {}

void editor_application::open_actor_prototype_editor() {
    m_proto_editor.open();
}
