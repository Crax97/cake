#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <typeinfo>
#include <list>
#include <utility>

#include "descriptor.h"
#include "extensions/std_extensions.h"
#include "field_visitor.h"
#include "glm/gtc/quaternion.hpp"


template <typename Type> void do_visit(Type &value, field_visitor &visitor) {
  assert(false && "NOT REACHED");
}

#define DECL_TYPE_INFO(Type)                                                        \
template <> descriptor *get_descriptor_typed<Type>() {                              \
class inner_descriptor : public descriptor {                                        \
    public:                                                                         \
    inner_descriptor(const std::string& name, [[maybe_unused]] size_t type_size):   \
        descriptor(name, nullptr) {}                                                \
        const std::type_info& get_typeinfo() const override {                       \
            return typeid(Type);                                                    \
        }                                                                           \
    };                                                                              \
    static inner_descriptor info{#Type, sizeof(Type)};                              \
    return &info;                                                                   \
  };

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
DEFINE_DO_VISIT(class pointer_field, visit_pointer_property)


class field {
private:
  std::string m_field_name;

protected:
  virtual void *get_impl(void *base, const std::type_info &info) = 0;
  virtual const void *get_impl(void *base,
                               const std::type_info &info) const = 0;

public:
  explicit field(std::string field_name)
      : m_field_name(std::move(field_name)) {}

  [[nodiscard]] const std::string &get_name() const { return m_field_name; }

    [[nodiscard]] virtual const descriptor *get_field_descriptor() const = 0;

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

  virtual ~field()= default;;
};

template <typename Class, typename Type>
class field_adder {
        class class_field : public field {
            Type Class::*m_field_ptr;

            Class &get_self(void *base) { return *static_cast<Class *>(base); }

            const Class &get_self(void *base) const {
                return *static_cast<Class *>(base);
            }

        public:
            class_field(std::string field_name, Type Class::*field_ptr)
                    : field(field_name), m_field_ptr(field_ptr) {}

            virtual void *get_impl(void *base, const std::type_info &info) override {
                assert(typeid(Type) == info &&
                       "Tried to get something with the wrong type!");
                assert(base != nullptr && "Tried to get a property from a null");
                return reinterpret_cast<void *>(&(get_self(base).*m_field_ptr));
            }


            [[nodiscard]] const descriptor *get_field_descriptor() const override {
                return get_descriptor_typed<Type>();
            }

            virtual const void *get_impl(void *base,
                                         const std::type_info &info) const override {
                assert(typeid(Type) == info &&
                       "Tried to get something with the wrong type!");
                assert(base != nullptr && "Tried to get a property from a null");
                return reinterpret_cast<const void *>(&(get_self(base).*m_field_ptr));
            }

            virtual std::string to_string(void *base) const noexcept override {
                return "TODO pointer lol";
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
public:
        void operator()(
                std::list<std::unique_ptr<field>> &fields, const std::string &member_name, Type Class::* ptr) {
            fields.emplace_back(std::make_unique<class_field>(member_name, ptr));
        }
};