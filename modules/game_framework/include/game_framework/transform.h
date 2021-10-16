#pragma once

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>

namespace spectacle {
class transform {
private:
  mutable glm::mat4 m_transform_matrix{glm::identity<glm::mat4>()};
  mutable bool m_needs_update{false};
  glm::vec3 m_location;
  glm::quat m_rotation;
  glm::vec3 m_scale;

public:
  transform()
      : m_location{0}, m_rotation{glm::identity<glm::quat>()}, m_scale{1} {}
  transform(const glm::vec3 &location, const glm::quat &rotation,
            const glm::vec3 &scale = glm::vec3(1.0f))
      : m_location{location}, m_rotation{rotation}, m_scale{scale} {
    m_needs_update = true;
  }
  transform(const transform &other)
      : m_location{other.m_location},
        m_rotation{other.m_rotation}, m_scale{other.m_scale} {
    update_transform_matrix();
  }
  transform(transform &&other)
      : m_location{std::move(other.m_location)}, m_rotation{std::move(
                                                     other.m_rotation)},
        m_scale{std::move(other.m_scale)}, m_needs_update{other.m_needs_update},
        m_transform_matrix{std::move(other.m_transform_matrix)} {}

  bool operator==(const transform &other) const noexcept;
  bool operator!=(const transform &other) const noexcept {
    return !(*this == other);
  }

  transform &operator=(const transform &other) noexcept;
  transform &operator=(transform &&other) noexcept;

  void set_location(const glm::vec3 &new_location) noexcept;
  void add_location_offset(const glm::vec3 &delta) noexcept;

  void set_rotation(const glm::quat &new_rotation) noexcept;
  void add_rotation_offset(const glm::quat &offset_rotation) noexcept;

  void set_scale(const glm::vec3 &new_scale) noexcept;
  void add_scale_offset(const glm::vec3 &scale_offset) noexcept;

  glm::vec3 get_forward() const noexcept;
  glm::vec3 get_left() const noexcept;
  glm::vec3 get_up() const noexcept;

  glm::vec3 &get_location() noexcept { return m_location; }
  glm::quat &get_rotation() noexcept { return m_rotation; }
  glm::vec3 &get_scale() noexcept { return m_scale; }

  const glm::vec3 &get_location() const noexcept { return m_location; }
  const glm::quat &get_rotation() const noexcept { return m_rotation; }
  const glm::vec3 &get_scale() const noexcept { return m_scale; }
  glm::mat4 get_transform_matrix() noexcept;

  bool needs_to_update() const noexcept { return m_needs_update; }

  void update_transform_matrix() noexcept;
};
} // namespace spectacle