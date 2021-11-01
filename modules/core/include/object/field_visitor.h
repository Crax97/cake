#pragma once

#include "glm/glm.hpp"
#include <string>

class field_visitor {
public:
  virtual void visit_bool_property(const bool &value) = 0;
  virtual void visit_char_property(const char &value) = 0;
  virtual void visit_int_property(const int &value) = 0;
  virtual void visit_double_property(const double &value) = 0;
  virtual void visit_float_property(const float &value) = 0;
  virtual void visit_string_property(const std::string &value) = 0;
  virtual void visit_vec2_property(const glm::vec2 &value) = 0;
  virtual void visit_vec3_property(const glm::vec3 &value) = 0;
  virtual void visit_vec4_property(const glm::vec4 &value) = 0;
  virtual void visit_quat_property(const glm::quat &value) = 0;
  virtual void visit_resource_property(const class resource& res) = 0;
  virtual void visit_pointer_property(const class pointer_field_base& field) = 0;
  virtual void visit_container_property(const class container_field& field) = 0;
};