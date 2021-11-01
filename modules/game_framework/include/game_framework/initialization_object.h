#pragma once

#include "extensions/std_extensions.h"
#include <iomanip>
#include <ostream>
#include <string>
#include <unordered_map>
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