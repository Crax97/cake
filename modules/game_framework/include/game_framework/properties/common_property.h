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

  void visit(void *base, property_visitor &visitor) override {
    auto &self = *static_cast<Klass *>(base);
    auto text_ptr = self.*m_ref;
    visitor.visit_texture_property(*text_ptr);
  }

public:
  explicit texture_property(
      const std::string &name,
      std::shared_ptr<renderer::texture> Klass::*ref) noexcept
      : property(name), m_ref(ref) {}
};
template <typename Klass> class int_property final : public property {
private:
  int Klass::*m_ref;

protected:
  virtual void set_impl(void *base, void *data, size_t data_size) override {
    assert(sizeof(int) == data_size);
    auto &self = *static_cast<Klass *>(base);
    self.*m_ref = *static_cast<int *>(data);
  }
  virtual void *get_impl(void *base, size_t data_size) override {
    assert(sizeof(int) == data_size);
    auto &self = *static_cast<Klass *>(base);
    return static_cast<void *>(&self.*m_ref);
  }
  void visit(void *base, property_visitor &visitor) override {
    auto &self = *static_cast<Klass *>(base);
    auto &value = self.*m_ref;
    visitor.visit_int_property(value);
  }

public:
  explicit int_property(const std::string &name, int Klass::*ref) noexcept
      : property(name), m_ref(ref) {}
};
template <typename Klass> class float_property final : public property {
private:
  float Klass::*m_ref;

protected:
  virtual void set_impl(void *base, void *data, size_t data_size) override {
    assert(sizeof(float) == data_size);
    auto &self = *static_cast<Klass *>(base);
    self.*m_ref = *static_cast<float *>(data);
  }
  virtual void *get_impl(void *base, size_t data_size) override {
    assert(sizeof(float) == data_size);
    auto &self = *static_cast<Klass *>(base);
    return static_cast<void *>(&self.*m_ref);
  }
  void visit(void *base, property_visitor &visitor) override {
    auto &self = *static_cast<Klass *>(base);
    auto &value = self.*m_ref;
    visitor.visit_float_property(value);
  }

public:
  explicit float_property(const std::string &name, float Klass::*ref) noexcept
      : property(name), m_ref(ref) {}
};
template <typename Klass> class double_property final : public property {
private:
  double Klass::*m_ref;

protected:
  virtual void set_impl(void *base, void *data, size_t data_size) override {
    assert(sizeof(double) == data_size);
    auto &self = *static_cast<Klass *>(base);
    self.*m_ref = *static_cast<double *>(data);
  }
  virtual void *get_impl(void *base, size_t data_size) override {
    assert(sizeof(double) == data_size);
    auto &self = *static_cast<Klass *>(base);
    return static_cast<void *>(&self.*m_ref);
  }
  void visit(void *base, property_visitor &visitor) override {
    auto &self = *static_cast<Klass *>(base);
    auto &value = self.*m_ref;
    visitor.visit_double_property(value);
  }

public:
  explicit double_property(const std::string &name, double Klass::*ref) noexcept
      : property(name), m_ref(ref) {}
};
template <typename Klass> class string_property final : public property {
private:
  std::string Klass::*m_ref;

protected:
  virtual void set_impl(void *base, void *data, size_t data_size) override {
    assert(sizeof(std::string) == data_size);
    auto &self = *static_cast<Klass *>(base);
    self.*m_ref = *static_cast<std::string *>(data);
  }
  virtual void *get_impl(void *base, size_t data_size) override {
    assert(sizeof(std::string) == data_size);
    auto &self = *static_cast<Klass *>(base);
    return static_cast<void *>(&self.*m_ref);
  }
  void visit(void *base, property_visitor &visitor) override {
    auto &self = *static_cast<Klass *>(base);
    auto &value = self.*m_ref;
    visitor.visit_string_property(value);
  }

public:
  explicit string_property(const std::string &name,
                           std::string Klass::*ref) noexcept
      : property(name), m_ref(ref) {}
};
template <typename Klass> class vec2_property final : public property {
private:
  glm::vec2 Klass::*m_ref;

protected:
  virtual void set_impl(void *base, void *data, size_t data_size) override {
    assert(sizeof(glm::vec2) == data_size);
    auto &self = *static_cast<Klass *>(base);
    self.*m_ref = *static_cast<glm::vec2 *>(data);
  }
  virtual void *get_impl(void *base, size_t data_size) override {
    assert(sizeof(int) == data_size);
    return static_cast<void *>(&m_ref);
  }

  void visit(void *base, property_visitor &visitor) override {
    auto &self = *static_cast<Klass *>(base);
    auto &value = self.*m_ref;
    visitor.visit_vec2_property(value);
  }

public:
  explicit vec2_property(const std::string &name,
                         glm::vec2 Klass::*ref) noexcept
      : property(name), m_ref(ref) {}
};
template <typename Klass> class vec3_property final : public property {
private:
  glm::vec3 Klass::*m_ref;

protected:
  virtual void set_impl(void *base, void *data, size_t data_size) override {
    assert(sizeof(glm::vec3) == data_size);
    auto &self = *static_cast<Klass *>(base);
    self.*m_ref = *static_cast<glm::vec3 *>(data);
  }
  virtual void *get_impl(void *base, size_t data_size) override {
    assert(sizeof(glm::vec3) == data_size);
    return static_cast<void *>(&m_ref);
  }

  void visit(void *base, property_visitor &visitor) override {
    auto &self = *static_cast<Klass *>(base);
    auto &value = self.*m_ref;
    visitor.visit_vec3_property(value);
  }

public:
  explicit vec3_property(const std::string &name,
                         glm::vec3 Klass::*ref) noexcept
      : property(name), m_ref(ref) {}
};
template <typename Klass> class vec4_property final : public property {
private:
  glm::vec4 Klass::*m_ref;

protected:
  virtual void set_impl(void *base, void *data, size_t data_size) override {
    assert(sizeof(glm::vec4) == data_size);
    auto &self = *static_cast<Klass *>(base);
    self.*m_ref = *static_cast<glm::vec4 *>(data);
  }
  virtual void *get_impl(void *base, size_t data_size) override {
    assert(sizeof(glm::vec4) == data_size);
    return static_cast<void *>(&m_ref);
  }

  void visit(void *base, property_visitor &visitor) override {
    auto &self = *static_cast<Klass *>(base);
    auto &value = self.*m_ref;
    visitor.visit_vec4_property(value);
  }

public:
  explicit vec4_property(const std::string &name,
                         glm::vec4 Klass::*ref) noexcept
      : property(name), m_ref(ref) {}
};
} // namespace property_system