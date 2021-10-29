//
// Created by crax on 10/30/21.
//

#pragma once

#include "object/object.h"
#include "object/field.h"
#include "object/macros.h"

#include <string_view>
#include <typeinfo>

class resource : public object {
GENERATE_REFLECT_BODY(resource)
protected:
public:
    [[nodiscard]] std::string to_string() const override = 0;
    virtual void from_string(std::string_view str) = 0;
    template<typename T>
    bool is_resource() {
        return typeid(*this) == typeid(T);
    }

    template<typename T>
    T& get() {
        assert(is_resource<T>());
        return *reinterpret_cast<T*>(this);
    }
};


template<typename Class>
class field_adder<Class, resource> {
    class class_field : public field {
        resource Class::*m_field_ptr;

        Class &get_self(void *base) { return *static_cast<Class *>(base); }

        const Class &get_self(const void *base) const {
            return *static_cast<const Class *>(base);
        }

    public:
        class_field(const std::string& field_name, resource Class::*field_ptr)
                : field(field_name), m_field_ptr(field_ptr) {}

        virtual void *get_impl(void *base, const std::type_info &info) override {
            assert(get_descriptor_typed<resource>()->get_typeinfo() == info &&
                   "Tried to get something with the wrong type!");
            assert(base != nullptr && "Tried to get a property from a null");
            return reinterpret_cast<void *>(&(get_self(base).*m_field_ptr));
        }


        [[nodiscard]] const descriptor *get_field_descriptor() const override {
            return get_descriptor_typed<resource>();
        }

        virtual const void *get_impl(void *base,
                                     const std::type_info &info) const override {
            assert(get_descriptor_typed<resource>()->get_typeinfo() == info &&
                   "Tried to get something with the wrong type!");
            assert(base != nullptr && "Tried to get a property from a null");
            return reinterpret_cast<const void *>(&(get_self(base).*m_field_ptr));
        }

        virtual std::string to_string(const void *base) const noexcept override {
            return get_self(base).to_string();
        }

        virtual void set_from_string(void *base,
                                     const std::string &value) noexcept override {
            get_self(base).from_string(value);
        }

        void visit(void *base, field_visitor &visitor) override {
            do_visit<resource>(*this, visitor);
        }

        ~class_field() override = default;
    };
public:
    void operator()(
            std::list<std::unique_ptr<field>> &fields, const std::string &member_name, resource Class::* ptr) {
        fields.emplace_back(std::make_unique<class_field>(member_name, ptr));
    }
};