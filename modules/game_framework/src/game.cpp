#include "game_framework/game.h"
#include "game_framework/actor.h"
#include "game_framework/camera.h"
#include "game_framework/rendering_world.h"
#include "game_framework/stage.h"

#include "game_framework/components/component_repository.h"
#include "game_framework/components/sprite_component.h"

#include "logging/logger.h"

#include <memory>

static logging::category core("core");

gameframework::game *g_game{nullptr};

using sprite_component = gameframework::sprite_component;
DEFINE_COMPONENT_REGISTRATOR(sprite_component)

gameframework::game::game(int argc, char **argv)
    : app_framework::application(argc, argv) {
  g_game = this;
  m_width = 1920;
  m_height = 1080;
}
void gameframework::game::on_app_startup() noexcept {
  m_camera = spectacle::camera(m_width, m_height, 75.0f);
  m_rendering_world =
      std::make_unique<gameframework::rendering_world>(get_api());
  m_stage = std::make_unique<spectacle::stage>();
  m_stage->on_stage_enter();
    core(logging::severity::info) << "Game loaded";
}
void gameframework::game::on_app_update() noexcept {
  update_game();
  render_world();
}
void gameframework::game::on_app_shutdown() noexcept {
  m_stage->on_stage_exit();
}

gameframework::rendering_world &
gameframework::game::get_rendering_world() noexcept {
  return *m_rendering_world;
}

void gameframework::game::update_game() noexcept {
  m_stage->on_stage_perform(0.0f);
}

void gameframework::game::render_world() noexcept {
  m_rendering_world->render(get_api(), m_camera);
}

gameframework::game &gameframework::game::the() noexcept { return *g_game; }
gameframework::game::~game() noexcept {}