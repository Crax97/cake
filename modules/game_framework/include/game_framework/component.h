#pragma once

#include "game_framework/properties/property.h"

#include "initialization_object.h"
#include <list>
#include <memory>

namespace spectacle {
class actor;
class component : public std::enable_shared_from_this<component> {
private:
  actor &m_owner;

protected:
  std::list<std::shared_ptr<property_system::property>> m_properties;

public:
  explicit component(actor &owner) : m_owner(owner) {}

  virtual void initialize(const initialization_object &object) noexcept {}
  virtual void begin_play() noexcept {}

  virtual void before_update(float delta_time) noexcept {}
  virtual void update(float delta_time) noexcept {}
  virtual void after_update(float delta_time) noexcept {}
  virtual void on_destroyed() noexcept {}

  virtual const char *get_component_name() const noexcept = 0;
  virtual std::shared_ptr<component> clone() const noexcept = 0;
  std::list<std::shared_ptr<property_system::property>> &
  get_editable_properties() {
    return m_properties;
  }

  actor &get_owner() noexcept { return m_owner; }

  virtual ~component() = default;
};
} // namespace spectacle