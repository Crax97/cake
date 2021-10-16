#pragma once

#include "glm/fwd.hpp"
#include "transform.h"

#include <memory>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace spectacle {
class component;
class actor {
  friend class stage;

private:
  bool m_is_pending_kill = false;
  std::unordered_map<std::type_index, std::shared_ptr<component>> m_components;

protected:
  transform m_transform;

public:
  bool is_enabled = true;
  bool tick_enabled = true;

  // When this actor enters the scene
  virtual void begin_play() noexcept;

  // Called before the actor updates his state
  virtual void before_update(float delta_time) noexcept;

  // Called each time the actor needs to update it's internal state
  virtual void update(float delta_time) noexcept;

  // Called after the actor updated his state
  virtual void after_update(float delta_time) noexcept;

  // Called when the actor is actually destroyed from the scene
  virtual void on_destroy() noexcept;

  void destroy() noexcept;
  bool is_pending_kill() const noexcept { return m_is_pending_kill; }

  void set_location(const glm::vec3 &new_location) noexcept;
  void set_rotation(const glm::quat &new_rotation) noexcept;
  void set_scale(const glm::vec3 &new_scale) noexcept;

  void add_location_offset(const glm::vec3 &location_offset) noexcept;
  void add_rotation_offset(const glm::quat &rotation_offset) noexcept;
  void add_scale_offset(const glm::vec3 &scale_offset) noexcept;

  glm::vec3 get_location() const noexcept;
  glm::quat get_rotation() const noexcept;
  glm::vec3 get_scale() const noexcept;

  transform &get_transform() noexcept { return m_transform; }

  template <typename T>
  void add_new_component(std::shared_ptr<T> new_component) noexcept {
    m_components.emplace(typeid(*new_component), std::move(new_component));
  }

  template <typename T> std::weak_ptr<T> get_component() noexcept {
    auto component = m_components.find(typeid(T));
    if (component == m_components.end())
      return nullptr;

    return component->second;
  }

  virtual ~actor() = default;
};
} // namespace spectacle