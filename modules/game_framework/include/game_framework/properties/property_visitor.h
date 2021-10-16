#pragma once

#include "glm/glm.hpp"
#include <string>

namespace renderer {
    class texture;
}

namespace property_system {
class property_visitor {
public:
  virtual void visit_int_property(int &value) = 0;
  virtual void visit_double_property(double &value) = 0;
  virtual void visit_float_property(float &value) = 0;
  virtual void visit_string_property(std::string &value) = 0;
  virtual void visit_vec2_property(glm::vec2 &value) = 0;
  virtual void visit_vec3_property(glm::vec3 &value) = 0;
  virtual void visit_vec4_property(glm::vec4 &value) = 0;
  virtual void visit_texture_property(renderer::texture &value) = 0;
};
} // namespace property_system