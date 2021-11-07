//
// Created by crax on 11/7/21.
//

#pragma once

#include "lua.hpp"

namespace luanatic {
    template<typename T>
    void push(lua_State* state, T value);

    template<typename T>
    T get(lua_State* state, int index);

    template<typename T>
    T pop(lua_State* state) {
        T val = get<T>(state, -1);
        lua_pop(state, 1);
        return val;
    }
}