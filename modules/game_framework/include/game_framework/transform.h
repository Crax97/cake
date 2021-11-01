#pragma once

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtx/quaternion.hpp"
#include <glm/fwd.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/mat4x4.hpp>

#include "object/field.h"
#include <memory>

namespace spectacle {
class transform {
private:
  mutable glm::mat4 m_transform_matrix{glm::identity<glm::mat4>()};
  mutable bool m_needs_update{false};
  glm::vec3 m_location;
  glm::quat m_rotation;
  glm::vec3 m_scale;

public:
  transform()
      : m_location{0}, m_rotation{glm::identity<glm::quat>()}, m_scale{1} {}
  transform(const glm::vec3 &location, const glm::quat &rotation,
            const glm::vec3 &scale = glm::vec3(1.0f))
      : m_location{location}, m_rotation{rotation}, m_scale{scale} {
    m_needs_update = true;
  }
  transform(const transform &other)
      : m_location{other.m_location},
        m_rotation{other.m_rotation}, m_scale{other.m_scale} {
    update_transform_matrix();
  }
  transform(transform &&other)
      : m_location{std::move(other.m_location)}, m_rotation{std::move(
                                                     other.m_rotation)},
        m_scale{std::move(other.m_scale)}, m_needs_update{other.m_needs_update},
        m_transform_matrix{std::move(other.m_transform_matrix)} {}

  bool operator==(const transform &other) const noexcept;
  bool operator!=(const transform &other) const noexcept {
    return !(*this == other);
  }

  transform &operator=(const transform &other) noexcept;
  transform &operator=(transform &&other) noexcept;

  void set_location(const glm::vec3 &new_location) noexcept;
  void add_location_offset(const glm::vec3 &delta) noexcept;

  void set_rotation(const glm::quat &new_rotation) noexcept;
  void add_rotation_offset(const glm::quat &offset_rotation) noexcept;

  void set_scale(const glm::vec3 &new_scale) noexcept;
  void add_scale_offset(const glm::vec3 &scale_offset) noexcept;

  glm::vec3 get_forward() const noexcept;
  glm::vec3 get_left() const noexcept;
  glm::vec3 get_up() const noexcept;

  glm::vec3 &get_location() noexcept { return m_location; }
  glm::quat &get_rotation() noexcept { return m_rotation; }
  glm::vec3 &get_scale() noexcept { return m_scale; }

  const glm::vec3 &get_location() const noexcept { return m_location; }
  const glm::quat &get_rotation() const noexcept { return m_rotation; }
  const glm::vec3 &get_scale() const noexcept { return m_scale; }
  glm::mat4 get_transform_matrix() noexcept;

  bool needs_to_update() const noexcept { return m_needs_update; }

  void update_transform_matrix() noexcept;

  template <typename Class>
  static std::unique_ptr<field>
  make_transform_field_location(transform Class::*m_transform_ptr) {
    class location_field : public field {
      transform Class::*m_field_ptr;

        Class &get_self(void *base) { return *static_cast<Class *>(base); }
      const Class &get_self(const void *base) const {
        return *static_cast<const Class *>(base);
      }

    public:
      location_field(transform Class::*field_ptr)
          : field("location"), m_field_ptr(field_ptr) {}

    [[nodiscard]] const descriptor *get_field_descriptor() const override {
          return get_descriptor_typed<glm::vec3>();
      }
      virtual void set_impl(void *base, const void* value, const std::type_info &info) override {
        assert(typeid(glm::vec3) == info &&
               "Tried to get something with the wrong type!");
        assert(base != nullptr && "Tried to get a property from a null");
        auto &my_transform = get_self(base).*m_field_ptr;
        my_transform.set_location(*static_cast<const glm::vec3*>(value));
      }
      virtual const void *get_impl(void *base,
                                   const std::type_info &info) const override {
        assert(typeid(glm::vec3) == info &&
               "Tried to get something with the wrong type!");
        assert(base != nullptr && "Tried to get a property from a null");
        auto &my_transform = get_self(base).*m_field_ptr;
        return reinterpret_cast<const void *>(&my_transform.m_location);
      }

      virtual std::string to_string(const void *base) const noexcept override {
        const auto &my_transform = get_self(base).*m_field_ptr;
        return std::to_string(my_transform.m_location);
      }
      virtual void set_from_string(void *base,
                                   const std::string &value) noexcept override {
        auto &my_transform = get_self(base).*m_field_ptr;
        my_transform.m_location = std::from_string<glm::vec3>(value);
      }

      virtual void visit(void *base, field_visitor &visitor) override {
        glm::vec3 &ref = (get_self(base).*m_field_ptr).m_location;
        return visitor.visit_vec3_property(ref);
      }

      ~location_field() {}
    };

    return std::make_unique<location_field>(m_transform_ptr);
  }
  template <typename Class>
  static std::unique_ptr<field>
  make_transform_field_rotation(transform Class::*m_transform_ptr) {
    class rotation_field : public field {
      transform Class::*m_field_ptr;

      Class &get_self(void *base) { return *static_cast<Class *>(base); }
      const Class &get_self(const void *base) const {
        return *static_cast<const Class*>(base);
      }

    public:
      rotation_field(transform Class::*field_ptr)
          : field("rotation"), m_field_ptr(field_ptr) {}
        [[nodiscard]] const descriptor *get_field_descriptor() const override {
            return get_descriptor_typed<glm::quat>();
        }
      virtual void set_impl(void *base, const void* value, const std::type_info &info) override {
        assert(typeid(glm::quat) == info &&
               "Tried to get something with the wrong type!");
        assert(base != nullptr && "Tried to get a property from a null");
        auto &my_transform = get_self(base).*m_field_ptr;
        my_transform.set_rotation(*static_cast<const glm::quat*>(value));
      }
      virtual const void *get_impl(void *base,
                                   const std::type_info &info) const override {
        assert(typeid(glm::quat) == info &&
               "Tried to get something with the wrong type!");
        assert(base != nullptr && "Tried to get a property from a null");
        auto &my_transform = get_self(base).*m_field_ptr;
        return reinterpret_cast<const void *>(&my_transform.m_rotation);
      }

      virtual std::string to_string(const void *base) const noexcept override {
        const auto &my_transform = get_self(base).*m_field_ptr;
        return std::to_string(glm::eulerAngles(my_transform.m_rotation));
      }
      virtual void set_from_string(void *base,
                                   const std::string &value) noexcept override {
        auto &my_transform = get_self(base).*m_field_ptr;
        auto euler = std::from_string<glm::vec3>(value);
        my_transform.m_rotation = glm::toQuat(glm::orientate3(euler));
      }

      virtual void visit(void *base, field_visitor &visitor) override {
        glm::quat &ref = (get_self(base).*m_field_ptr).m_rotation;
        return visitor.visit_quat_property(ref);
      }
      ~rotation_field() {}
    };

    return std::make_unique<rotation_field>(m_transform_ptr);
  }
  template <typename Class>
  static std::unique_ptr<field>
  make_transform_field_scale(transform Class::*m_transform_ptr) {
    class scale_field : public field {
      transform Class::*m_field_ptr;

      Class &get_self(void *base) { return *static_cast<Class *>(base); }
      const Class &get_self(const void *base) const {
        return *static_cast<const Class *>(base);
      }

    public:
      scale_field(transform Class::*field_ptr)
          : field("scale"), m_field_ptr(field_ptr) {}
        [[nodiscard]] const descriptor *get_field_descriptor() const override {
            return get_descriptor_typed<glm::vec3>();
        }
      virtual void set_impl(void *base, const void* value, const std::type_info &info) override {
        assert(typeid(glm::vec3) == info &&
               "Tried to get something with the wrong type!");
        assert(base != nullptr && "Tried to get a property from a null");
        auto &my_transform = get_self(base).*m_field_ptr;
        my_transform.set_scale(*static_cast<const glm::vec3*>(value));
      }
      virtual const void *get_impl(void *base,
                                   const std::type_info &info) const override {
        assert(typeid(glm::vec3) == info &&
               "Tried to get something with the wrong type!");
        assert(base != nullptr && "Tried to get a property from a null");
        auto &my_transform = get_self(base).*m_field_ptr;
        return reinterpret_cast<const void *>(&my_transform.m_scale);
      }

      virtual std::string to_string(const void *base) const noexcept override {
        const auto &my_transform = get_self(base).*m_field_ptr;
        return std::to_string(my_transform.m_scale);
      }
      virtual void set_from_string(void *base,
                                   const std::string &value) noexcept override {
        auto &my_transform = get_self(base).*m_field_ptr;
        my_transform.m_scale = std::from_string<glm::vec3>(value);
      }

      virtual void visit(void *base, field_visitor &visitor) override {
        glm::vec3 &ref = (get_self(base).*m_field_ptr).m_scale;
        return visitor.visit_vec3_property(ref);
      }
      ~scale_field() {}
    };

    return std::make_unique<scale_field>(m_transform_ptr);
  }
};
} // namespace spectacle
template<typename Class>
class field_adder<Class, spectacle::transform> {

public:
    void operator()(
            std::list<std::unique_ptr<field>> &fields, const std::string &member_name, spectacle::transform Class::* ptr) {
        auto loc_field = spectacle::transform::make_transform_field_location(ptr);
        auto rot_field = spectacle::transform::make_transform_field_rotation(ptr);
        auto scale_field = spectacle::transform::make_transform_field_scale(ptr);
        fields.emplace_back(std::move(loc_field));
        fields.emplace_back(std::move(rot_field));
        fields.emplace_back(std::move(scale_field));
    }
};