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
        if constexpr(std::is_pointer_v<T>) {
            T val = get_pointer<T>(state, -1);
            lua_pop(state, 1);
            return val;
        } else {
            T val = get<T>(state, -1);
            lua_pop(state, 1);
            return val;
        }
    }

    template<typename T>
    inline T pop_pointer(lua_State* state) {
        T val = get_pointer<T>(state, -1);
        lua_pop(state, 1);
        return val;
    }
    template<typename T>
    inline void push_pointer(lua_State* state, T val) {
        T* ptr = reinterpret_cast<T*>(lua_newuserdata(state, sizeof(T)));
        *ptr = val;
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

    template<typename T>
    void bind(lua_State* state, std::string_view name, T value) {
        if constexpr (std::is_pointer_v<T>) {
            push_pointer(state, value);
        } else {
            push(value);
        }
        lua_setglobal(state, name.data());
    }

    template<typename Return, typename... Args>
    void bind(lua_State* state, std::string_view name, Return (*fun)(Args...)) {
        using sig = Return (*)(Args...);
        sig* ptr = reinterpret_cast<sig*>(lua_newuserdata(state, sizeof(sig*)));
        *ptr = fun;

        auto caller = [](lua_State* state) {
            sig fn = *reinterpret_cast<sig*>(lua_touserdata(state, 1));
            return call_helper<Return, Args...>(fn, state, std::index_sequence_for<Args...>{});
        };

        lua_newtable(state);
        push(state, +caller);
        lua_setfield(state, -2, "__call");
        lua_setmetatable(state, -2);
        lua_setglobal(state, name.data());
    }
    inline void bind(lua_State* state, std::string_view name, lua_CFunction fun) {
        lua_pushcfunction(state, fun);
        lua_setglobal(state, name.data());
    }

    // this tests the metatable as well, so make sure you set the appropriate metatable on the userdata
    template<typename Class>
    Class* get_self(lua_State* state, int self_idx = 1) {
        return *reinterpret_cast<Class**>(luaL_testudata(state, self_idx, get_metatable_name<Class>()));
    }

}