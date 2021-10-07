#include "transform.h"

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/transform.hpp"

spectacle::transform &
spectacle::transform::operator=(const transform &other) noexcept {
  m_location = other.m_location;
  m_rotation = other.m_rotation;
  m_scale = other.m_scale;
  update_transform_matrix();
  return *this;
}
spectacle::transform &
spectacle::transform::operator=(transform &&other) noexcept {
  m_location = std::move(other.m_location);
  m_rotation = std::move(other.m_rotation);
  m_scale = std::move(other.m_scale);
  m_needs_update = other.m_needs_update;
  m_transform_matrix = std::move(other.m_transform_matrix);
  return *this;
}

void spectacle::transform::update_transform_matrix() noexcept {
  m_transform_matrix = glm::scale(m_scale) * glm::toMat4(m_rotation) *
                       glm::translate(m_location);
}
void spectacle::transform::set_location(
    const glm::vec3 &new_location) noexcept {
  m_needs_update = true;
  m_location = new_location;
}
void spectacle::transform::add_location_offset(
    const glm::vec3 &delta) noexcept {
  m_needs_update = true;
  m_location += delta;
}
void spectacle::transform::set_rotation(
    const glm::quat &new_rotation) noexcept {
  m_needs_update = true;
  m_rotation = new_rotation;
}
void spectacle::transform::add_rotation_offset(
    const glm::quat &offset_rotation) noexcept {
  m_needs_update = true;
  m_rotation *= offset_rotation;
}
void spectacle::transform::set_scale(const glm::vec3 &new_scale) noexcept {
  m_needs_update = true;
  m_scale = new_scale;
}
void spectacle::transform::add_scale_offset(
    const glm::vec3 &scale_offset) noexcept {
  m_needs_update = true;
  m_scale += scale_offset;
}
glm::mat4 spectacle::transform::get_transform_matrix() noexcept {
  if (m_needs_update) {
    m_needs_update = false;
    update_transform_matrix();
  }
  return m_transform_matrix;
}

glm::vec3 spectacle::transform::get_forward() const noexcept {
  if (m_needs_update) {
    m_needs_update = false;
    const_cast<transform *>(this)->update_transform_matrix();
  }
  return m_transform_matrix[2];
}
glm::vec3 spectacle::transform::get_left() const noexcept {
  if (m_needs_update) {
    m_needs_update = false;
    const_cast<transform *>(this)->update_transform_matrix();
  }
  return m_transform_matrix[0];
}
glm::vec3 spectacle::transform::get_up() const noexcept {
  if (m_needs_update) {
    m_needs_update = false;
    const_cast<transform *>(this)->update_transform_matrix();
  }
  return m_transform_matrix[1];
}