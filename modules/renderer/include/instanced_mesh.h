#pragma once

#include "mesh.h"

#include <memory>
#include <string>
#include <utility>

namespace renderer {
enum class attrib_data_type {
  Float,
  Double,
  Byte,
  Int,
  Unsigned_Byte,
  Unsigned_Int
};

enum class attrib_element_count { One, Two, Three, Four, Mat2, Mat3, Mat4 };

struct attrib_descriptor {
  std::string attrib_name;
  attrib_data_type type;
  attrib_element_count el_count;
  size_t offset;
};
class instance_descriptor {
public:
  virtual std::vector<attrib_descriptor> get_descriptors() = 0;
  virtual size_t get_stride() = 0;
  virtual size_t get_element_size() = 0;

  virtual ~instance_descriptor() = default;
};
class instanced_mesh : public mesh {
protected:
  std::unique_ptr<instance_descriptor> m_descriptor;
  std::shared_ptr<mesh> m_mesh;

  virtual void add_instance_impl(void *instance_ptr) noexcept = 0;
  virtual void *
  map_instance_buffer_impl(const size_t min_buffer_size_bytes) noexcept = 0;

public:
  instanced_mesh(std::shared_ptr<mesh> mesh,
                 std::unique_ptr<instance_descriptor> &&descriptor)
      : m_descriptor(std::move(descriptor)), m_mesh(std::move(mesh)) {}

  virtual bool configure_for_shader(
      std::shared_ptr<class shader> requiredShader) noexcept = 0;

  void bind() const noexcept override { m_mesh->bind(); }
  [[nodiscard]] size_t element_count() const noexcept override {
    return m_mesh->element_count();
  }

  virtual void clear() noexcept = 0;

  virtual void unmap_instance_buffer() = 0;
  template <typename T> void add_instance(T element) noexcept {
    add_instance_impl(static_cast<void *>(&element));
  }
  template <typename T>
  T *map_instance_buffer(const size_t min_num_elements) noexcept {
    return static_cast<T *>(map_instance_buffer_impl(min_num_elements));
  }
};
} // namespace renderer