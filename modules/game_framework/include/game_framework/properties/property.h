#pragma once

#include "property_visitor.h"

#include <cstddef>
namespace property_system {
class property {
protected:
  virtual void set_impl(void *base, void *data, size_t data_size) = 0;
  virtual void *get_impl(void *base, size_t data_size) = 0;

public:
  template <typename T> void set(void *base, T &&value) noexcept {
    set_impl(base, static_cast<void *>(&value), sizeof(T));
  }
  template <typename T> T &get(void *base) noexcept {
    return *static_cast<T *>(get_impl(base, sizeof(T)));
  }
  virtual void visit(void *base, property_visitor &visitor) = 0;
};
} // namespace property_system