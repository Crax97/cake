#include "game_framework/components/rendering_component.h"
#include "game_framework/actor.h"
#include "game_framework/component.h"
#include "game_framework/game.h"

#include "game_framework/rendering_world.h"
#include <memory>

void gameframework::rendering_component::begin_play() noexcept {
    component::begin_play();
    gameframework::game::the().get_rendering_world().register_rendering_component(
            this);
}

void gameframework::rendering_component::on_destroyed() noexcept {
    component::on_destroyed();
    gameframework::game::the()
            .get_rendering_world()
            .unregister_rendering_component(this);
}
