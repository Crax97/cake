//
// Created by crax on 11/7/21.
//

#pragma once

#include "lua.hpp"
#include <type_traits>
#include <typeinfo>
#include <string>
#include <utility>

namespace luanatic {

    void print_stack(lua_State* state);

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

    template<typename Return, typename... Args, size_t... I>
    int call_helper(Return (*fun)(Args...), lua_State* state, std::index_sequence<I...>) {
        if constexpr(std::is_void_v<Return>) {
            (*fun)(luanatic::get<Args>(state, -static_cast<int>(sizeof...(Args) - I))...);
            return 0;
        } else {
            push(state, (*fun)(luanatic::get<Args>(state, -static_cast<int>(sizeof...(Args) - I))...));
            return 1;
        }
    }

    // this tests the metatable as well, so make sure you set the appropriate metatable on the userdata
    template<typename Class>
    Class* get_self(lua_State* state) {
        // lmao rtfm next time
        constexpr int LUA_SELF_INDEX = 1;
        return *reinterpret_cast<Class**>(luaL_testudata(state, LUA_SELF_INDEX, get_metatable_name<Class>()));
    }
}