#pragma once

#include "application.h"
#include <memory>

namespace spectacle {
class stage;
}

namespace gameframework {
class rendering_world;
class game : public app_framework::application {

protected:
  std::unique_ptr<spectacle::stage> m_stage{nullptr};

  std::unique_ptr<rendering_world> m_rendering_world;

  virtual void update_game() noexcept;
  virtual void render_world() noexcept;

public:
  game(int argc, char **argv);
  virtual void on_app_startup() noexcept override;
  virtual void on_app_update() noexcept override;
  virtual void on_app_shutdown() noexcept override;

  rendering_world &get_rendering_world() noexcept;

  static game &the() noexcept;
  virtual ~game() noexcept;
};
} // namespace gameframework