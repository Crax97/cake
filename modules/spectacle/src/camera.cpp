#include "camera.h"
#include "glm/ext/matrix_clip_space.hpp"

spectacle::camera::camera(int width, int height, float fov) noexcept {
  m_ortographic =
      glm::ortho(width * 0.5f, -width * 0.5f, -height * 0.5f, height * 0.5f);
  m_perspective = glm::perspective(
      glm::radians(fov), static_cast<float>(width) / height, 0.01f, 1000.0f);
  current_mode = camera_mode::Ortographic;
  update_view_matrix();
}

void spectacle::camera::update_view_matrix() noexcept {
  const auto forward = m_transform.get_forward();
  m_view =
      glm::lookAt(m_transform.get_location(),
                  m_transform.get_location() + forward, glm::vec3(0, 1, 0));
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