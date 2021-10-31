#pragma once

#include "game_framework/component.h"
#include "renderer/renderer.hpp"

namespace renderer {
class graphics_api;
class texture;
} // namespace renderer

namespace gameframework {
class actor;
class rendering_component : public spectacle::component {
public:
    explicit rendering_component(spectacle::actor& owner)
        : spectacle::component(owner) { }
    void begin_play() noexcept override;
    void on_destroyed() noexcept override;

  virtual void draw(renderer::renderer &renderer) noexcept = 0;

};
} // namespace gameframework