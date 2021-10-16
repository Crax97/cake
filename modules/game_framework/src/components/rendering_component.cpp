#include "game_framework/components/rendering_component.h"
#include "game_framework/actor.h"
#include "game_framework/component.h"
#include "game_framework/game.h"

#include "game_framework/rendering_world.h"
#include <memory>

gameframework::rendering_component::rendering_component(spectacle::actor &owner)
    : spectacle::component(owner) {
  gameframework::game::the().get_rendering_world().register_rendering_component(
      this);
}
gameframework::rendering_component::~rendering_component() noexcept {
  gameframework::game::the()
      .get_rendering_world()
      .unregister_rendering_component(this);
}