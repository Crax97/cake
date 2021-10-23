#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <typeinfo>

#include "extensions/std_extensions.h"
#include "field_visitor.h"
#include "glm/gtc/quaternion.hpp"

class type_descriptor {
private:
  std::string m_name;
  size_t m_size;

public:
  type_descriptor(const std::string &name, size_t size)
      : m_name(name), m_size(size) {}
  const std::string &get_name() const { return m_name; }
  size_t get_size() { return m_size; }
};

template <typename T> type_descriptor *get_type() {
  assert(false && "This should not be reached");
  return nullptr;
}

#define DEFINE_TYPE_INFO(Type) template <> type_descriptor *get_type<Type>();

DEFINE_TYPE_INFO(bool)
DEFINE_TYPE_INFO(char)
DEFINE_TYPE_INFO(unsigned char)
DEFINE_TYPE_INFO(int)
DEFINE_TYPE_INFO(unsigned int)
DEFINE_TYPE_INFO(long)
DEFINE_TYPE_INFO(unsigned long)
DEFINE_TYPE_INFO(unsigned long long)
DEFINE_TYPE_INFO(float)
DEFINE_TYPE_INFO(double)
DEFINE_TYPE_INFO(glm::vec2)
DEFINE_TYPE_INFO(glm::vec3)
DEFINE_TYPE_INFO(glm::vec4)
DEFINE_TYPE_INFO(glm::quat)

template <typename Type> void do_visit(Type &value, field_visitor &visitor) {
  assert(false && "NOT REACHED");
}

#define DEFINE_DO_VISIT(Type, fn)                                              \
  template <>                                                                  \
  inline void do_visit<Type>(Type & value, field_visitor & visitor) {          \
    visitor.fn(value);                                                         \
  }

DEFINE_DO_VISIT(bool, visit_bool_property)
DEFINE_DO_VISIT(char, visit_char_property)
DEFINE_DO_VISIT(int, visit_int_property)
DEFINE_DO_VISIT(float, visit_float_property)
DEFINE_DO_VISIT(double, visit_double_property)
DEFINE_DO_VISIT(glm::vec2, visit_vec2_property)
DEFINE_DO_VISIT(glm::vec3, visit_vec3_property)
DEFINE_DO_VISIT(glm::vec4, visit_vec4_property)

#define DECL_TYPE_INFO(Type)                                                   \
  template <> type_descriptor *get_type<Type>() {                              \
    static type_descriptor info{#Type, sizeof(Type)};                          \
    return &info;                                                              \
  };

class field {
private:
  std::string m_field_name;
  type_descriptor *m_type;

protected:
  virtual void *get_impl(void *base, const std::type_info &info) = 0;
  virtual const void *get_impl(void *base,
                               const std::type_info &info) const = 0;

public:
  explicit field(const std::string &field_name, type_descriptor *type)
      : m_field_name(field_name), m_type(type) {}

  const std::string &get_name() const { return m_field_name; }
  const type_descriptor &get_type_info() const { return *m_type; }

  template <typename T> T &get(void *base) {
    return *static_cast<T *>(get_impl(base, typeid(T)));
  }
  template <typename T> const T &get(void *const base) const {
    return *static_cast<T *>(get_impl(base, typeid(T)));
  }

  virtual std::string to_string(void *base) const noexcept = 0;
  virtual void set_from_string(void *base,
                               const std::string &value) noexcept = 0;
  virtual void visit(void *base, field_visitor &visitor) = 0;

  virtual ~field(){};
};
template <typename Class, typename Type>
std::unique_ptr<field> make_class_field(const std::string &member_name,
                                        Type Class::*ptr) {
  class class_field : public field {
    Type Class::*m_field_ptr;

    Class &get_self(void *base) { return *static_cast<Class *>(base); }
    const Class &get_self(void *base) const {
      return *static_cast<Class *>(base);
    }

  public:
    class_field(std::string field_name, Type Class::*field_ptr)
        : field(field_name, get_type<Type>()), m_field_ptr(field_ptr) {}
    virtual void *get_impl(void *base, const std::type_info &info) override {
      assert(typeid(Type) == info &&
             "Tried to get something with the wrong type!");
      assert(base != nullptr && "Tried to get a property from a null");
      return reinterpret_cast<void *>(&(get_self(base).*m_field_ptr));
    }
    virtual const void *get_impl(void *base,
                                 const std::type_info &info) const override {
      assert(typeid(Type) == info &&
             "Tried to get something with the wrong type!");
      assert(base != nullptr && "Tried to get a property from a null");
      return reinterpret_cast<const void *>(&(get_self(base).*m_field_ptr));
    }

    virtual std::string to_string(void *base) const noexcept override {
      return std::to_string(get_self(base).*m_field_ptr);
    }
    virtual void set_from_string(void *base,
                                 const std::string &value) noexcept override {
      get_self(base).*m_field_ptr = std::from_string<Type>(value);
    }

    virtual void visit(void *base, field_visitor &visitor) override {
      Type &ref = get_self(base).*m_field_ptr;
      return do_visit<Type>(ref, visitor);
    }

    ~class_field() {}
  };
  return std::make_unique<class_field>(member_name, ptr);
}