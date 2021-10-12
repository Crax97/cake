#include "game_framework/game.h"
#include "actor.h"
#include "game_framework/rendering_world.h"
#include "stage.h"

#include "game_framework/components/component_repository.h"
#include "game_framework/components/sprite_component.h"

#include <memory>

gameframework::game *g_game{nullptr};

using SpriteComponent = gameframework::sprite_component;
DEFINE_COMPONENT_REGISTRATOR(SpriteComponent)

gameframework::game::game(int argc, char **argv)
    : app_framework::application(argc, argv) {
  g_game = this;
  m_width = 1920;
  m_height = 1080;
}
void gameframework::game::on_app_startup() noexcept {
  m_rendering_world =
      std::make_unique<gameframework::rendering_world>(get_api());
  m_stage = std::make_unique<spectacle::stage>();
  m_stage->on_stage_enter();
}
void gameframework::game::on_app_update() noexcept {}
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
  m_rendering_world->render(get_api());
}

gameframework::game &gameframework::game::the() noexcept { return *g_game; }
gameframework::game::~game() noexcept {}