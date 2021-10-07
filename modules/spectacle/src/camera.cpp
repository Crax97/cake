#include "camera.h"

void spectacle::camera::update_view_matrix() noexcept {
  const auto forward = m_transform.get_forward();
  m_view =
      glm::lookAt(m_transform.get_location(),
                  m_transform.get_location() + forward, glm::vec3(0, 0, 1));
}

glm::mat4 spectacle::camera::get_view_matrix() noexcept {
  if (m_transform.needs_to_update()) {
    m_transform.update_transform_matrix();
    update_view_matrix();
  }
  return m_view;
}

glm::mat4 spectacle::camera::get_projection_matrix() const noexcept {
  if (current_mode == camera_mode::Ortographic) {
    return m_ortographic;
  }
  return m_perspective;
}