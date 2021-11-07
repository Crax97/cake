#pragma once

#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <type_traits>

#include "extensions/std_extensions.h"
#include "field_visitor.h"
#include "glm/gtc/quaternion.hpp"
#include "logging/logger.h"

// If you're reaching this with Type = std::shared_ptr<T>
// please ensure you're including pointer_field.h before calling
// field_adder<Class, std::shared_ptr>
template <typename Type> void do_visit(const Type &value, field_visitor &visitor);

#define DEFINE_DO_VISIT(Type, fn)                                              \
  template <>                                                                  \
  inline void do_visit<Type>(const Type & value, field_visitor & visitor) {          \
    visitor.fn(value);                                                         \
  }

DEFINE_DO_VISIT(bool, visit_bool_property)
DEFINE_DO_VISIT(char, visit_char_property)
DEFINE_DO_VISIT(int, visit_int_property)
DEFINE_DO_VISIT(float, visit_float_property)
DEFINE_DO_VISIT(double, visit_double_property)
DEFINE_DO_VISIT(std::string, visit_string_property)
DEFINE_DO_VISIT(glm::vec2, visit_vec2_property)
DEFINE_DO_VISIT(glm::vec3, visit_vec3_property)
DEFINE_DO_VISIT(glm::vec4, visit_vec4_property)
DEFINE_DO_VISIT(class pointer_field_base, visit_pointer_property)
DEFINE_DO_VISIT(class container_field, visit_container_property)
DEFINE_DO_VISIT(class resource, visit_resource_property)

namespace object_utilities {
// Utilities to check if a T satisfies certain conditions
    template<typename T>
    constexpr bool is_smart_pointer = false;
    template<typename T>
    constexpr bool is_smart_pointer<std::shared_ptr<T>> = true;
    template<typename T>
    constexpr bool is_smart_pointer<std::unique_ptr<T>> = true;

    template<typename T>
    struct smart_pointer_type {};
    template<typename T>
    struct smart_pointer_type<std::shared_ptr<T>> {
        using type = std::shared_ptr<T>;
    };

    template<typename T>
    struct smart_pointer_type<std::unique_ptr<T>> {
        using type = std::unique_ptr<T>;
    };

    template<typename T>
    struct vector_inner_type {
    };
    template<typename T>
    struct vector_inner_type<std::vector<T>> {
        using type = T;
    };

    template<typename T>
    constexpr bool is_vector = false;
    template<typename T>
    constexpr bool is_vector<std::vector<T>> = true;

    template<typename T>
    constexpr bool is_resource = std::is_base_of<resource, T>::value;
}

class field {
protected:
    virtual const void *get_impl(void *base,
                               const std::type_info &info) const = 0;
    virtual void set_impl(void *base, const void* value, const std::type_info &info) = 0;

public:
    [[nodiscard]] virtual const std::type_info& get_type() const = 0;

  template <typename T> const T &get(void *const base) const {
    return *static_cast<const T *>(get_impl(base, typeid(T)));
  }

  template <typename U> void set(void* base, const U& value) {
      return set_impl(base, &value, typeid(U));
  }


    virtual std::string to_string(const void *base) const noexcept = 0;
  virtual void set_from_string(void *base,
                               const std::string &value) noexcept = 0;
  virtual void visit(void *base, field_visitor &visitor) = 0;

  virtual ~field();
};


template<typename T>
field* field_of() {
    class simple_field : public field {
    public:
        [[nodiscard]] const std::type_info& get_type()const override {
            return typeid(T);
        }

        std::string to_string(const void *base) const noexcept override {
            if constexpr(std::is_constructible_v<std::string, T>) {
                return std::string(*static_cast<const T *>(base));
            } else {
                return std::to_string(*static_cast<const T *>(base));
            }
        }

        void set_from_string(void *base, const std::string &value) noexcept override {
            *static_cast<T *>(base) = std::from_string<T>(value);
        }

        void visit(void *base, field_visitor &visitor) override {
            do_visit<T>(*static_cast<T *>(base), visitor);
        }

    protected:
        const void *get_impl(void *base, const std::type_info &info) const override {
            return base;
        }

        void set_impl(void *base, const void *value, const std::type_info &info) override {
            MIKU_ASSERT(typeid(T) == info);
            *static_cast<T *>(base) = *static_cast<const T *>(value);
        }
    };
    static simple_field field;
    return &field;
}

template<typename Class, typename Type>
class class_field : public field {
    Type Class::*m_field_ptr;

    Class &get_self(void *base) { return *static_cast<Class *>(base); }

    const Class &get_self(const void *base) const {
        return *static_cast<const Class *>(base);
    }

public:
    class_field(Type Class::*field_ptr)
            : m_field_ptr(field_ptr) {}


    [[nodiscard]] const std::type_info& get_type()const override {
        return typeid(Type);
    }

    const void *get_impl(void *base,
                                 const std::type_info &info) const override {
        MIKU_ASSERT(typeid(Type) == info &&
               "Tried to get something with the wrong type!");
        MIKU_ASSERT(base != nullptr && "Tried to get a property from a null");
        return reinterpret_cast<const void *>(&(get_self(base).*m_field_ptr));
    }

    void set_impl(void *base, const void* value, const std::type_info &info) override {
        MIKU_ASSERT(typeid(Type) == info &&
                    "Tried to set something with the wrong type!");
        MIKU_ASSERT(base != nullptr && "Tried to get a property from a null");
        get_self(base).*m_field_ptr = *static_cast<const Type* const>(value);
    }

    std::string to_string(const void *base) const noexcept override {
        auto& self = get_self(base).*m_field_ptr;
        if constexpr(object_utilities::is_resource<Type>) {
            return self.to_string();
        } else if constexpr(std::is_same_v<Type, std::string>) {
            return self;
        } else {
            return std::to_string(self);
        }
    }

    void set_from_string(void *base,
                                 const std::string &value) noexcept override {
        auto& self = get_self(base).*m_field_ptr;
        if constexpr(object_utilities::is_resource<Type>) {
            self.from_string(value);
        } else if constexpr(std::is_same_v<Type, std::string>) {
            self = value;
        } else {
            self = std::from_string<Type>(value);
        }
    }

    void visit(void *base, field_visitor &visitor) override {
        Type &ref = get_self(base).*m_field_ptr;
        return do_visit<Type>(ref, visitor);
    }

    ~class_field() override = default;
};

class pointer_field_base : public field {
public:
    explicit pointer_field_base(const std::string& name) { }
    [[nodiscard]] virtual const std::type_info& get_inner_type() const = 0;
    template<typename T>
    [[nodiscard]] bool points_to() const {
        return typeid(T) == get_inner_type();
    }

    template<typename T>
    std::shared_ptr<T> get_ptr(void* base) const {
        MIKU_ASSERT(points_to<T>());
        return get<std::shared_ptr<T>>(base);
    }
};

template<typename Class, typename T>
class pointer_field : public pointer_field_base {
    using pointer_type = typename object_utilities::smart_pointer_type<T>::type;
    pointer_type Class::*m_field_ptr;

    Class &get_self(void *base) { return *static_cast<Class *>(base); }

    const Class &get_self(const void *base) const {
        return *static_cast<const Class *>(base);
    }

public:
    pointer_field(const std::string& field_name, pointer_type Class::*field_ptr)
            : pointer_field_base(field_name), m_field_ptr(field_ptr) {}

    virtual void set_impl(void *base, const void* value, const std::type_info &info) override {
        MIKU_ASSERT(typeid(pointer_type) == info &&
               "Tried to get something with the wrong type!");
        MIKU_ASSERT(base != nullptr && "Tried to get a property from a null");
        get_self(base).*m_field_ptr = static_cast<const pointer_type*>(value);
    }


    [[nodiscard]] const std::type_info& get_type() const override {
        return typeid(pointer_type);
    }

    [[nodiscard]] const std::type_info& get_inner_type() const override {
        return typeid(T);
    }

    virtual const void *get_impl(void *base,
                                 const std::type_info &info) const override {
        MIKU_ASSERT(typeid(pointer_type) == info &&
               "Tried to get something with the wrong type!");
        MIKU_ASSERT(base != nullptr && "Tried to get a property from a null");
        return reinterpret_cast<const void *>(&(get_self(base).*m_field_ptr));
    }

    virtual std::string to_string(const void *base) const noexcept override {
        auto ptr = get_self(base).*m_field_ptr;
        if(ptr) {
            return ptr->to_string();
        }
        return "null";
    }

    virtual void set_from_string(void *base,
                                 const std::string &value) noexcept override {
        auto ptr = get_self(base).*m_field_ptr;
        if(ptr) {
            ptr->from_string(value);
        }
    }

    void visit(void *base, field_visitor &visitor) override {
        do_visit<pointer_field>(*this, visitor);
    }

    ~pointer_field() override = default;
};


class container_field : public field {
protected:
    virtual void* get_instance_internal(void* base, int index)  = 0;

public:
    explicit container_field() { }
    [[nodiscard]] virtual const std::type_info& get_inner_type() const = 0;
    template<typename T>
    [[nodiscard]] bool contains() const {
        return typeid(T) == get_inner_type();
    }

    template<typename T>
    T& get_instance(void* base, int index) {
        MIKU_ASSERT(contains<T>());
        return *static_cast<T*>(get_instance_internal(base, index));
    }
};

template<typename Class, typename VecType>
class vector_field : public container_field {
    VecType Class::*m_field_ptr;
    using inner_type = typename object_utilities::vector_inner_type<VecType>::type;
    Class &get_self(void *base) { return *static_cast<Class *>(base); }

    const Class &get_self(const void *base) const {
        return *static_cast<const Class *>(base);
    }
    const VecType& get_field(const void *base) const {
        auto& self = get_self(base);
        return self.*m_field_ptr;
    }
protected:
    void* get_instance_internal(void* base, int index) override {
        return (void*)(&get_field(base)[index]);
    }

public:
    explicit vector_field(VecType Class::*field_ptr)
            : m_field_ptr(field_ptr) {}

    void set_impl(void *base, const void* value, const std::type_info &info) override {
        MIKU_ASSERT(typeid(VecType) == info &&
               "Tried to get something with the wrong type!");
        MIKU_ASSERT(base != nullptr && "Tried to get a property from a null");
        get_self(base).*m_field_ptr = *static_cast<const VecType*>(value);
    }


    [[nodiscard]] const std::type_info& get_inner_type() const override {
        return typeid(inner_type);
    }
    [[nodiscard]] const std::type_info& get_type() const override {
        return typeid(VecType);
    }

    const void *get_impl(void *base,
                         const std::type_info &info) const override {
        MIKU_ASSERT(typeid(VecType) == info &&
               "Tried to get vector_type with the wrong type!");
        MIKU_ASSERT(base != nullptr && "Tried to get a property from a null");
        return reinterpret_cast<const void *>(&(get_self(base).*m_field_ptr));
    }

    std::string to_string(const void *base) const noexcept override {
        std::stringstream str;
        str << "[ ";
        auto& inner_vec = get_field(base);
        for(const auto& ele : inner_vec) {
            if constexpr(object_utilities::is_smart_pointer<inner_type>) {
                str << *ele << ", ";
            } else {
                str << ele << ", ";
            }
        }
        str << "]";
        return str.str();
    }

    void set_from_string(void *base,
                         const std::string &value) noexcept override {
        // get_self(base).*m_field_ptr = std::from_string<Type>(value);
    }

    void visit(void *base, field_visitor &visitor) override {
        visitor.visit_container_property(*this);
    }

    ~vector_field() override = default;
};

template <typename Class, typename Type>
class field_adder {

public:
        void operator()(std::unordered_map<std::string, std::unique_ptr<field>> &fields, const std::string &member_name, Type Class::* ptr) {
            if constexpr(object_utilities::is_smart_pointer<Type>) {
                fields.emplace(member_name, std::make_unique<pointer_field<Class, Type>>(ptr));
            } else if constexpr(object_utilities::is_vector<Type>) {
                fields.emplace(member_name, std::make_unique<vector_field<Class, Type>>(ptr));
            } else {
                fields.emplace(member_name, std::make_unique<class_field<Class, Type>>(ptr));
            }
        }
};


template<typename T>
concept is_stringable_val = requires(T t) {
    {t.to_string()};
};
template<typename T>
concept is_stringable_ptr = requires(T t) {
    {t->to_string()};
};

template<typename T>
concept is_object = (is_stringable_ptr<T> || is_stringable_val<T>);