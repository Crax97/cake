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

  std::list<std::shared_ptr<actor>> &get_all_actors() noexcept {
    return m_actor_list;
  }

  template <typename Functor> void for_each_actor(Functor &&functor) noexcept {
    std::for_each(m_actor_list.begin(), m_actor_list.end(), functor);
  }

  virtual ~stage() = default;
};
} // namespace spectacle