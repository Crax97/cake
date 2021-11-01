#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "logging/logger.h"

#include "glm/glm.hpp"

class object;

namespace std {
template <typename T> T from_string(const std::string &s) {
  MIKU_ASSERT(false && "This shouldn't be reached");
  return T();
}

double read_number(std::istream& stream);

string to_string(const glm::vec2 &vec);
string to_string(const glm::vec3 &vec);
string to_string(const glm::vec4 &vec);
string to_string(const glm::quat &quat);
std::string to_string(const class object& obj);

} // namespace std