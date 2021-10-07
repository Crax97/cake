#pragma once

#include <list>
#include <memory>

namespace spectacle {
class actor;
class stage {
private:
  std::list<std::shared_ptr<actor>> m_actor_list;

public:
  // Called before everything else
  virtual void on_stage_enter() noexcept;

  // Called every update
  virtual void on_stage_perform(float delta_seconds) noexcept;

  // Called when the stage needs to release its resources
  virtual void on_stage_exit() noexcept;

  virtual ~stage() = default;
};
} // namespace spectacle