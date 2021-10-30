#include "extensions/std_extensions.h"

#include "object/object.h"
#include "object/fields.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include <string>

namespace std {
template <> std::string from_string<std::string>(const std::string &val) {
  return val;
}
template <> char from_string(const std::string &val) { return val[0]; }
template <> unsigned char from_string(const std::string &val) { return val[0]; }
template <> short from_string(const std::string &val) { return std::stoi(val); }
template <> unsigned short from_string(const std::string &val) {
  return std::stoi(val);
}
template <> int from_string(const std::string &val) { return std::stoi(val); }
template <> unsigned int from_string(const std::string &val) {
  return std::stoul(val);
}

template <> long from_string(const std::string &val) { return std::stol(val); }
template <> long long from_string(const std::string &val) {
  return std::stoll(val);
}
template <> unsigned long from_string(const std::string &val) {
  return std::stoul(val);
}
template <> unsigned long long from_string(const std::string &val) {
  return std::stoull(val);
}
template <> bool from_string(const std::string &val) { return val == "true"; }

template <> float from_string(const std::string &val) { return std::stof(val); }
template <> double from_string(const std::string &val) {
  return std::stod(val);
}
template <> glm::vec2 from_string(const std::string &val) {
  glm::vec2 vec;
  std::sscanf(val.c_str(), "[%f,%f]", &vec.x, &vec.y);
  return vec;
}

template <> glm::vec3 from_string(const std::string &val) {
  glm::vec3 vec;
  std::sscanf(val.c_str(), "[%f,%f,%f]", &vec.x, &vec.y, &vec.z);
  return vec;
}

template <> glm::vec4 from_string(const std::string &val) {
  glm::vec4 vec;
  std::sscanf(val.c_str(), "[%f,%f,%f,%f]", &vec.x, &vec.y, &vec.z, &vec.w);
  return vec;
}

template <> glm::quat from_string(const std::string &val) {
  auto vec = from_string<glm::vec4>(val);
  return glm::quat(vec);
}

std::string to_string(const object &obj) {
    return obj.to_string();
}

std::string to_string(const glm::vec2 &vec) {
  stringstream str;
  str.precision(5);
  str << std::fixed <<"[" << vec.x << ", " << vec.y << "]";
  return str.str();
}

std::string to_string(const glm::vec3 &vec) {
  stringstream str;
  str.precision(5);
  str <<std::fixed << "[" << (float)vec.x << ", " << (float)vec.y << ", " << (float)vec.z << "]";
  return str.str();
}

std::string to_string(const glm::vec4 &vec) {
  stringstream str;
  str.precision(5);
  str << std::fixed << "[" << (float)vec.x << ", " << (float)vec.y << ", " << (float)vec.z << ", " << (float)vec.w << "]";
  return str.str();
}

std::string to_string(const glm::quat &quat) {
  return to_string(glm::vec4(quat.x, quat.y, quat.z, quat.w));
}
} // namespace std
