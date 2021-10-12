#include "stage.h"
#include "actor.h"
#include "utils.h"

template <spectacle::detail::Iterable It,
          std::invocable<spectacle::actor &> Functor>
void for_each_actor_ref(It &&it, Functor &&fun) {
  for (auto &actor : it) {
    fun(*actor);
  }
}

void spectacle::stage::on_stage_enter() noexcept {
  for_each_actor_ref(m_actor_list, [](actor &a) { a.begin_play(); });
}
void spectacle::stage::on_stage_perform(float delta_seconds) noexcept {
  // Remove pending kill actors
  std::erase_if(m_actor_list, [](std::shared_ptr<actor> a) {
    // ugly hack
    if (a->is_pending_kill()) {
      a->on_destroy();
      return true;
    }
    return false;
  });

  // Update each actor
  for_each_actor_ref(m_actor_list, [delta_seconds](actor &a) {
    a.before_update(delta_seconds);
  });

  for_each_actor_ref(m_actor_list,
                     [delta_seconds](actor &a) { a.update(delta_seconds); });

  for_each_actor_ref(m_actor_list, [delta_seconds](actor &a) {
    a.after_update(delta_seconds);
  });
}

void spectacle::stage::add_new_actor(
    std::shared_ptr<spectacle::actor> new_actor) noexcept {
  m_actor_list.push_back(std::move(new_actor));
}

void spectacle::stage::on_stage_exit() noexcept {
  for_each_actor_ref(m_actor_list, [](actor &a) {
    a.destroy();
    a.on_destroy();
  });
  m_actor_list.clear();
}