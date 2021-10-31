#pragma once

#include "object/object.h"

#include "initialization_object.h"
#include <list>
#include <memory>

namespace spectacle {
class actor;
class component : public object, public std::enable_shared_from_this<component> {
private:
  actor &m_owner;
    bool m_marked_for_removal = false;
public:
  explicit component(actor &owner) : m_owner(owner) {}

  void mark_for_removal() noexcept {
      m_marked_for_removal = true;
  }

  bool marked_for_removal() const {
      return m_marked_for_removal;
  }

  virtual void initialize(const initialization_object &object) noexcept {}
  virtual void begin_play() noexcept {}

  virtual void before_update(float delta_time) noexcept {}
  virtual void update(float delta_time) noexcept {}
  virtual void after_update(float delta_time) noexcept {}
  virtual void on_destroyed() noexcept {}

  virtual std::shared_ptr<component> clone(spectacle::actor& new_owner) const noexcept = 0;

  actor &get_owner() noexcept { return m_owner; }

  virtual ~component() = default;
};
} // namespace spectacle