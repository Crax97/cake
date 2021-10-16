#pragma once
#include "property.h"

#include "glm/glm.hpp"
#include <cstddef>
#include <string>
namespace property_system {
#if 0
template <typename Klass> class int_property final : public property {
private:
  int Klass::*m_ref;

protected:
  virtual void set_impl(void *base, void *data, size_t data_size) override {
    assert(sizeof(int) == data_size);
    m_ref = *static_cast<int *>(data);
  }
  virtual void *get_impl(void *base, size_t data_size) override {
    assert(sizeof(int) == data_size);
    return static_cast<void *>(&m_ref);
  }

public:
  explicit int_property(int &ref) noexcept : m_ref(ref) {}
};
template <typename Klass> 
class float_property final : public property {
private:
  float Klass::* m_ref;

protected:
  virtual void set_impl(void *data, size_t data_size) override {
    assert(sizeof(float) == data_size);
    m_ref = *static_cast<float *>(data);
  }
  virtual void *get_impl(size_t data_size) override {
    assert(sizeof(float) == data_size);
    return static_cast<void *>(&m_ref);
  }

public:
  explicit float_property(float &ref) noexcept : m_ref(ref) {}
};
class double_property final : public property {
private:
  double &m_ref;

protected:
  virtual void set_impl(void *data, size_t data_size) override {
    assert(sizeof(double) == data_size);
    m_ref = *static_cast<double *>(data);
  }
  virtual void *get_impl(size_t data_size) override {
    assert(sizeof(double) == data_size);
    return static_cast<void *>(&m_ref);
  }

public:
  explicit double_property(double &ref) noexcept : m_ref(ref) {}
};
class string_property final : public property {
private:
  std::string &m_ref;

protected:
  virtual void set_impl(void *data, size_t data_size) override {
    assert(sizeof(std::string) == data_size);
    m_ref = *static_cast<std::string *>(data);
  }
  virtual void *get_impl(size_t data_size) override {
    assert(sizeof(std::string) == data_size);
    return static_cast<void *>(&m_ref);
  }

public:
  explicit string_property(std::string &ref) noexcept : m_ref(ref) {}
};
class vec2_property final : public property {
private:
  glm::vec2 &m_ref;

protected:
  virtual void set_impl(void *data, size_t data_size) override {
    assert(sizeof(glm::vec2) == data_size);
    m_ref = *static_cast<glm::vec2 *>(data);
  }
  virtual void *get_impl(size_t data_size) override {
    assert(sizeof(glm::vec2) == data_size);
    return static_cast<void *>(&m_ref);
  }

public:
  explicit vec2_property(glm::vec2 &ref) noexcept : m_ref(ref) {}
};
class vec3_property final : public property {
private:
  glm::vec3 &m_ref;

protected:
  virtual void set_impl(void *data, size_t data_size) override {
    assert(sizeof(glm::vec3) == data_size);
    m_ref = *static_cast<glm::vec3 *>(data);
  }
  virtual void *get_impl(size_t data_size) override {
    assert(sizeof(glm::vec3) == data_size);
    return static_cast<void *>(&m_ref);
  }

public:
  explicit vec3_property(glm::vec3 &ref) noexcept : m_ref(ref) {}
};
class vec4_property final : public property {
private:
  glm::vec4 &m_ref;

protected:
  virtual void set_impl(void *data, size_t data_size) override {
    assert(sizeof(glm::vec4) == data_size);
    m_ref = *static_cast<glm::vec4 *>(data);
  }
  virtual void *get_impl(size_t data_size) override {
    assert(sizeof(glm::vec4) == data_size);
    return static_cast<void *>(&m_ref);
  }

public:
  explicit vec4_property(glm::vec4 &ref) noexcept : m_ref(ref) {}
};
#endif
} // namespace property_system