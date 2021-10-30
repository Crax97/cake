#include "game_framework/actor.h"

#include "game_framework/component.h"
#include "object/descriptor.h"
#include "object/object.h"
#include "utils.h"

#include <algorithm>
#include <concepts>
#include <type_traits>

#include "actor.gen.h"

void spectacle::actor::begin_play() noexcept {
  for_each_component([](auto &component) { component.begin_play(); });
}

void spectacle::actor::before_update(float delta_time) noexcept {
  for_each_component(
      [delta_time](auto &component) { component.before_update(delta_time); });
}

void spectacle::actor::update(float delta_time) noexcept {
  for_each_component(
      [delta_time](auto &component) { component.update(delta_time); });
}

void spectacle::actor::after_update(float delta_time) noexcept {
  for_each_component(
      [delta_time](auto &component) { component.after_update(delta_time); });
}

void spectacle::actor::destroy() noexcept { m_is_pending_kill = true; }

void spectacle::actor::on_destroy() noexcept {
  for_each_component([](auto &component) { component.on_destroyed(); });
}

void spectacle::actor::set_location(const glm::vec3 &new_location) noexcept {
  m_transform.set_location(new_location);
}
void spectacle::actor::set_rotation(const glm::quat &new_rotation) noexcept {
  m_transform.set_rotation(new_rotation);
}
void spectacle::actor::set_scale(const glm::vec3 &new_scale) noexcept {
  m_transform.set_rotation(new_scale);
}

void spectacle::actor::add_location_offset(
    const glm::vec3 &location_offset) noexcept {
  m_transform.add_location_offset(location_offset);
}
void spectacle::actor::add_rotation_offset(
    const glm::quat &rotation_offset) noexcept {
  m_transform.add_rotation_offset(rotation_offset);
}
void spectacle::actor::add_scale_offset(
    const glm::vec3 &scale_offset) noexcept {
  m_transform.add_scale_offset(scale_offset);
}

glm::vec3 spectacle::actor::get_location() const noexcept {
  return m_transform.get_location();
}
glm::quat spectacle::actor::get_rotation() const noexcept {
  return m_transform.get_rotation();
}
glm::vec3 spectacle::actor::get_scale() const noexcept {
  return m_transform.get_scale();
}

void spectacle::actor::serialize(serializer &ser) const {
    ser.begin_section("actor");
    ser.add_parameter("prototype", "\"TODO\"");
    ser.add_parameter("location", std::to_string(get_location()));
    ser.add_parameter("rotation", std::to_string(get_rotation()));
    ser.add_parameter("scale", std::to_string(get_scale()));
    for(const auto& component : m_components) {
        component.second->serialize(ser);
    }
    ser.end_section("actor");
}
