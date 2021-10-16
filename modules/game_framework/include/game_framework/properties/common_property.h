#pragma once
#include "property.h"

#include "game_framework/properties/property.h"
#include "glm/glm.hpp"
#include "texture.h"
#include <cassert>
#include <cstddef>
#include <memory>
#include <string>
namespace property_system {

template <typename Klass>
class texture_property final : public property_system::property {
private:
  std::shared_ptr<renderer::texture> Klass::*m_ref;

protected:
  virtual void set_impl(void *base, void *data, size_t data_size) override {
    assert(sizeof(renderer::texture) == data_size);
    assert(base != nullptr);
    auto &self = *static_cast<Klass *>(base);
    // self.*m_ref = *static_cast<renderer::texture *>(data);
  }
  virtual void *get_impl(void *base, size_t data_size) override {
    assert(sizeof(renderer::texture) == data_size);
    assert(base != nullptr);
    auto &self = *static_cast<Klass *>(base);
    return nullptr;
  }

public:
  explicit texture_property(
      std::shared_ptr<renderer::texture> Klass::*ref) noexcept
      : m_ref(ref) {}
  void visit(void *base, property_visitor &visitor) override {
    auto &self = *static_cast<Klass *>(base);
    auto text_ptr = self.*m_ref;
    visitor.visit_texture_property(*text_ptr);
  }
};
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
    auto &self = *static_cast<Klass *>(base);
    return static_cast<void *>(&self.*m_ref);
  }

public:
  explicit int_property(int &ref) noexcept : m_ref(ref) {}
};
template <typename Klass> class float_property final : public property {
private:
  float Klass::*m_ref;

protected:
  virtual void set_impl(void *base, void *data, size_t data_size) override {
    assert(sizeof(int) == data_size);
    m_ref = *static_cast<float *>(data);
  }
  virtual void *get_impl(void *base, size_t data_size) override {
    assert(sizeof(int) == data_size);
    auto &self = *static_cast<Klass *>(base);
    return static_cast<void *>(&self.*m_ref);
  }

public:
  explicit float_property(float &ref) noexcept : m_ref(ref) {}
};
template <typename Klass> class double_property final : public property {
private:
  double Klass::*m_ref;

protected:
  virtual void set_impl(void *base, void *data, size_t data_size) override {
    assert(sizeof(int) == data_size);
    m_ref = *static_cast<double *>(data);
  }
  virtual void *get_impl(void *base, size_t data_size) override {
    assert(sizeof(int) == data_size);
    auto &self = *static_cast<Klass *>(base);
    return static_cast<void *>(&self.*m_ref);
  }

public:
  explicit double_property(double &ref) noexcept : m_ref(ref) {}
};
template <typename Klass> class string_property final : public property {
private:
  std::string Klass::*m_ref;

protected:
  virtual void set_impl(void *base, void *data, size_t data_size) override {
    assert(sizeof(int) == data_size);
    m_ref = *static_cast<std::string *>(data);
  }
  virtual void *get_impl(void *base, size_t data_size) override {
    assert(sizeof(int) == data_size);
    auto &self = *static_cast<Klass *>(base);
    return static_cast<void *>(&self.*m_ref);
  }

public:
  explicit string_property(std::string &ref) noexcept : m_ref(ref) {}
};
template <typename Klass> class vec2_property final : public property {
private:
  glm::vec2 Klass::*m_ref;

protected:
  virtual void set_impl(void *base, void *data, size_t data_size) override {
    assert(sizeof(int) == data_size);
    m_ref = *static_cast<glm::vec2 *>(data);
  }
  virtual void *get_impl(void *base, size_t data_size) override {
    assert(sizeof(int) == data_size);
    return static_cast<void *>(&m_ref);
  }

public:
  explicit vec2_property(glm::vec2 &ref) noexcept : m_ref(ref) {}
};
template <typename Klass> class vec3_property final : public property {
private:
  glm::vec3 Klass::*m_ref;

protected:
  virtual void set_impl(void *base, void *data, size_t data_size) override {
    assert(sizeof(int) == data_size);
    m_ref = *static_cast<glm::vec3 *>(data);
  }
  virtual void *get_impl(void *base, size_t data_size) override {
    assert(sizeof(int) == data_size);
    return static_cast<void *>(&m_ref);
  }

public:
  explicit vec3_property(glm::vec3 &ref) noexcept : m_ref(ref) {}
};
template <typename Klass> class vec4_property final : public property {
private:
  glm::vec4 Klass::*m_ref;

protected:
  virtual void set_impl(void *base, void *data, size_t data_size) override {
    assert(sizeof(int) == data_size);
    m_ref = *static_cast<glm::vec4 *>(data);
  }
  virtual void *get_impl(void *base, size_t data_size) override {
    assert(sizeof(int) == data_size);
    return static_cast<void *>(&m_ref);
  }

public:
  explicit vec4_property(glm::vec4 &ref) noexcept : m_ref(ref) {}
};
} // namespace property_system