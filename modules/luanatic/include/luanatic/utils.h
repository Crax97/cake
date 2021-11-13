//
// Created by crax on 11/7/21.
//

#pragma once

#include "lua.hpp"
#include <type_traits>
#include <typeinfo>
#include <string>

namespace luanatic {
    template<typename Class>
    const char* get_metatable_name() {
        static std::string meta_name = std::string("metatable_") + std::string(typeid(Class).name());
        return meta_name.c_str();
    }

    template<typename T>
    void push(lua_State* state, const T& value);

    template<typename T>
    T get(lua_State* state, int index = -1);

    template<typename T>
    inline T get_pointer(lua_State* state, int index) {
        return reinterpret_cast<T>(lua_touserdata(state, index));
    }

    template<typename T>
    T pop(lua_State* state) {
        T val = get<T>(state, -1);
        lua_pop(state, 1);
        return val;
    }

    template<typename T>
    inline T pop_pointer(lua_State* state) {
        T val = get_pointer<T>(state, -1);
        lua_pop(state, 1);
        return val;
    }
}