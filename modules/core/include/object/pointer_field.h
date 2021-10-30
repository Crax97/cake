//
// Created by crax on 10/26/21.
//
#pragma once

#include "field.h"
#include "descriptor.h"

#include <memory>
#include <cassert>

class pointer_field : public field {
public:
    explicit pointer_field(const std::string& name)
        : field(name) { }
    [[nodiscard]] virtual const descriptor* get_inner_type() const = 0;
    template<typename T>
    [[nodiscard]] bool points_to() const {
        return typeid(T) == get_inner_type()->get_typeinfo();
    }

    template<typename T>
    std::shared_ptr<T> get_ptr(void* base) {
        assert(points_to<T>());
        return get<std::shared_ptr<T>>(base);
    }
};

template<typename Class, is_object T>
class field_adder<Class, std::shared_ptr<T>> {
    using pointer_type = std::shared_ptr<T>;
    class class_field : public pointer_field {
        std::shared_ptr<T> Class::*m_field_ptr;

        Class &get_self(void *base) { return *static_cast<Class *>(base); }

        const Class &get_self(const void *base) const {
            return *static_cast<const Class *>(base);
        }

    public:
        class_field(const std::string& field_name, pointer_type Class::*field_ptr)
                : pointer_field(field_name), m_field_ptr(field_ptr) {}

        virtual void *get_impl(void *base, const std::type_info &info) override {
            assert(typeid(pointer_type) == info &&
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
            assert(typeid(pointer_type) == info &&
                   "Tried to get something with the wrong type!");
            assert(base != nullptr && "Tried to get a property from a null");
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

        ~class_field() override = default;
    };
public:
    void operator()(
            std::list<std::unique_ptr<field>> &fields, const std::string &member_name, pointer_type Class::* ptr) {
        fields.emplace_back(std::make_unique<class_field>(member_name, ptr));
    }
};