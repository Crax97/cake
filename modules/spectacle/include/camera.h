#pragma once

#include "actor.h"

namespace spectacle {
enum class camera_mode { Perspective, Ortographic };

class camera : public actor {

private:
  glm::mat4 m_view;
  glm::mat4 m_perspective;
  glm::mat4 m_ortographic;

  void update_view_matrix() noexcept;

public:
  camera_mode current_mode;

  glm::mat4 get_view_matrix() noexcept;
  glm::mat4 get_projection_matrix() const noexcept;
  glm::mat4 get_view_projection_matrix() noexcept {
    return get_projection_matrix() * get_view_matrix();
  }
};
} // namespace spectacle