//
// Created by crax on 10/26/21.
//
#pragma once

#include "field.h"
#include "descriptor.h"

#include <vector>
#include <cassert>

namespace detail {
    template <typename T>
    struct is_smart_ptr {
        static constexpr bool value = false;
    };
    template <typename T>
    struct is_smart_ptr<std::shared_ptr<T>> {
        static constexpr bool value = true;
        using Inner = T;
    };
    template <typename T>
    struct is_smart_ptr<std::unique_ptr<T>> {
        static constexpr bool value = true;
        using Inner = T;
    };
    template <typename T>
    struct is_smart_ptr<std::weak_ptr<T>> {
        static constexpr bool value = true;
        using Inner = T;
    };
}
class container_field : public field {
protected:
    virtual void* get_instance_internal(void* base, int index)  = 0;

public:
    explicit container_field(const std::string& name)
        : field(name) { }
    [[nodiscard]] virtual const descriptor* get_inner_type() const = 0;
    template<typename T>
    [[nodiscard]] bool contains() const {
        return typeid(T) == get_inner_type()->get_typeinfo();
    }

    template<typename T>
    T& get_instance(void* base, int index) {
        assert(contains<T>());
        return *static_cast<T*>(get_instance_internal(base, index));
    }
};


template<typename Class, typename T>
class field_adder<Class, std::vector<T>> {
    using vector_type = std::vector<T>;
    class class_field : public container_field {
        vector_type Class::*m_field_ptr;

        Class &get_self(void *base) { return *static_cast<Class *>(base); }

        const Class &get_self(void *base) const {
            return *static_cast<Class *>(base);
        }
        const vector_type& get_field(void* base) const {
            auto& self = get_self(base);
            return self.*m_field_ptr;
        }
    protected:
        void* get_instance_internal(void* base, int index) override {
            return (void*)(&get_field(base)[index]);
        }

    public:
        class_field(const std::string& field_name, vector_type Class::*field_ptr)
                : container_field(field_name), m_field_ptr(field_ptr) {}

        virtual void *get_impl(void *base, const std::type_info &info) override {
            assert(typeid(vector_type) == info &&
                   "Tried to get something with the wrong type!");
            assert(base != nullptr && "Tried to get a property from a null");
            return reinterpret_cast<void *>(&(get_self(base).*m_field_ptr));
        }


        [[nodiscard]] const descriptor *get_field_descriptor() const override {
            return get_descriptor_typed<T>();
        }
        [[nodiscard]] const descriptor *get_inner_type() const override {
            return get_descriptor_typed<T>();
        }

        virtual const void *get_impl(void *base,
                                     const std::type_info &info) const override {
            assert(typeid(vector_type) == info &&
                   "Tried to get vector_type with the wrong type!");
            assert(base != nullptr && "Tried to get a property from a null");
            return reinterpret_cast<const void *>(&(get_self(base).*m_field_ptr));
        }

        virtual std::string to_string(void *base) const noexcept override {
            std::stringstream str;
            str << "[ ";
            auto& inner_vec = get_field(base);
            for(const auto& ele : inner_vec) {
                if constexpr(detail::is_smart_ptr<T>::value) {
                    str << *ele << ", ";
                } else {
                    str << ele << ", ";
                }
            }
            str << "]";
            return str.str();
        }

        virtual void set_from_string(void *base,
                                     const std::string &value) noexcept override {
            // get_self(base).*m_field_ptr = std::from_string<Type>(value);
        }

        void visit(void *base, field_visitor &visitor) override {
            visitor.visit_container_property(*this);
        }

        ~class_field() override = default;
    };
public:
    void operator()(
            std::list<std::unique_ptr<field>> &fields, const std::string &member_name, vector_type Class::* ptr) {
        fields.emplace_back(std::make_unique<class_field>(member_name, ptr));
    }
};