#pragma once

#include <vector>
#include <memory>

#include "object/object.h"
#include "object/macros.h"

namespace spectacle {
class actor;
class REFLECT() stage : public object {
    GENERATE_REFLECT_BODY(stage)
private:
  std::vector<std::shared_ptr<actor>> REFLECT() m_actor_list;

public:
  // Called before everything else
  virtual void on_stage_enter() noexcept;

  // Called every update
  virtual void on_stage_perform(float delta_seconds) noexcept;

  // Called when the stage needs to release its resources
  virtual void on_stage_exit() noexcept;

  std::vector<std::shared_ptr<actor>> &get_all_actors() noexcept {
    return m_actor_list;
  }

  template <typename Functor> void for_each_actor(Functor &&functor) noexcept {
    std::for_each(m_actor_list.begin(), m_actor_list.end(), functor);
  }

  void add_new_actor(std::shared_ptr<actor> new_actor) noexcept;

  virtual ~stage() = default;
};
} // namespace spectacle