#pragma once

#include "glm/glm.hpp"
#include <string>

class field_visitor {
public:
  virtual void visit_bool_property(bool &value) = 0;
  virtual void visit_char_property(char &value) = 0;
  virtual void visit_int_property(int &value) = 0;
  virtual void visit_double_property(double &value) = 0;
  virtual void visit_float_property(float &value) = 0;
  virtual void visit_string_property(std::string &value) = 0;
  virtual void visit_vec2_property(glm::vec2 &value) = 0;
  virtual void visit_vec3_property(glm::vec3 &value) = 0;
  virtual void visit_vec4_property(glm::vec4 &value) = 0;
  virtual void visit_quat_property(glm::quat &value) = 0;
  virtual void visit_resource_property(class resource& res) = 0;
  virtual void visit_pointer_property(class pointer_field& field) = 0;
  virtual void visit_container_property(class container_field& field) = 0;
};