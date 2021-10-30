#pragma once

#include <cassert>
#include <sstream>
#include <string>
#include <vector>

#include "glm/glm.hpp"

class object;

namespace std {
template <typename T> T from_string(const std::string &s) {
  assert(false && "This shouldn't be reached");
  return T();
}

double read_number(std::istream& stream);

string to_string(const glm::vec2 &vec);
string to_string(const glm::vec3 &vec);
string to_string(const glm::vec4 &vec);
string to_string(const glm::quat &quat);
std::string to_string(const class object& obj);

} // namespace std