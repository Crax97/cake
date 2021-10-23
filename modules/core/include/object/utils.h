#pragma once

#include <cassert>

namespace std {
template <typename T> T from_string(const std::string &s) {
  assert(false && "This shouldn't be reached");
  return T();
}

} // namespace std
