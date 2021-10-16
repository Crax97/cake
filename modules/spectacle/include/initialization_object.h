#pragma once

#include <cassert>
#include <iomanip>
#include <ostream>
#include <string>
#include <unordered_map>

namespace std {
template <typename T> T from_string(const std::string &_val) {
  assert(false);
  std::exit(-1);
}
template <>
inline std::string from_string<std::string>(const std::string &val) {
  return val;
}
template <> inline int from_string(const std::string &val) {
  return std::stoi(val);
}
template <> inline unsigned int from_string(const std::string &val) {
  return std::stoul(val);
}

template <> inline long from_string(const std::string &val) {
  return std::stol(val);
}
template <> inline unsigned long from_string(const std::string &val) {
  return std::stoul(val);
}
template <> inline unsigned long long from_string(const std::string &val) {
  return std::stoull(val);
}
template <> inline bool from_string(const std::string &val) {
  return val == "true";
}

template <> inline char from_string(const std::string &val) { return val[0]; }
template <> inline unsigned char from_string(const std::string &val) {
  return val[0];
}
template <> inline float from_string(const std::string &val) {
  return std::stof(val);
}
template <> inline double from_string(const std::string &val) {
  return std::stod(val);
}
} // namespace std

namespace spectacle {
class initialization_object {
private:
  std::unordered_map<std::string, std::string> m_values;

public:
  template <typename S>
  void add(const std::string &key, const S &value) noexcept {
    m_values.emplace(key, std::to_string(value));
  }

  template <typename S> S get(const std::string &key) const noexcept {
    auto find = m_values.find(key);
    if (find != m_values.end()) {
      return std::from_string<S>(find->second);
    }
    return S();
  }

  std::ostream &operator<<(std::ostream &out_stream) {
    for (const auto &pair : m_values) {
      out_stream << std::quoted(pair.first) << " : " << std::quoted(pair.second)
                 << "\n";
    }
    return out_stream;
  }
};

template <>
inline void
initialization_object::add<std::string>(const std::string &key,
                                        const std::string &value) noexcept {
  m_values.emplace(key, value);
}
} // namespace spectacle