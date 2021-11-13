//
// Created by crax on 11/13/21.
//
#pragma once
#include "lua.hpp"

namespace luanatic {
    struct lua_field {
        virtual void get(void *base, lua_State *state) = 0;

        virtual void set(void *base, lua_State *state) = 0;
    };

    enum class field_accessibility {
        read = 0x01,
        write = 0x02,
        read_and_write = read | write,
    };

    template<typename Class, typename T>
    lua_field* make_field_for_class(T Class::* field, field_accessibility accessibility) {
        struct inner_getter : public lua_field {
            using field_type = T Class::*;
            field_type m_field;
            field_accessibility m_accessibility = field_accessibility::read_and_write;
            inner_getter(field_type ffield, field_accessibility aaccessibility)
                    : m_field(ffield), m_accessibility(aaccessibility) { }
            void get(void* base, lua_State* state) override {
                if(!(static_cast<int>(m_accessibility) & static_cast<int>(field_accessibility::read))) {
                    luaL_error(state, "Tried to access a field which is not readable");
                }
                auto* instance = reinterpret_cast<Class*>(base);
                push<T>(state, instance->*m_field);
            }
            void set(void* base, lua_State* state) override {
                if(!(static_cast<int>(m_accessibility) & static_cast<int>(field_accessibility::write))) {
                    luaL_error(state, "Tried to write a field which is not writable");
                }
                auto* instance = reinterpret_cast<Class*>(base);
                (instance->*m_field) = luanatic::get<T>(state);
            }
        };

        return new inner_getter(field, accessibility);
    }

}