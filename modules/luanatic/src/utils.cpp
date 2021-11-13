//
// Created by crax on 11/7/21.
//

#include "luanatic/utils.h"

#include <string>

namespace luanatic {
    template<>
    void push<int>(lua_State *state, const int& value) {
        lua_pushinteger(state, value);
    }
    template<>
    void push<float>(lua_State *state, const float& value) {
        lua_pushnumber(state, value);
    }
    template<>
    void push<double>(lua_State *state, const double& value) {
        lua_pushnumber(state, value);
    }
    template<>
    void push<std::string>(lua_State *state, const std::string& value) {
        lua_pushstring(state, value.c_str());
    }
    template<>
    void push<std::string_view>(lua_State *state, const std::string_view& value) {
        lua_pushstring(state, value.data());
    }
    template<>
    void push<lua_CFunction>(lua_State *state, const lua_CFunction& value) {
        lua_pushcfunction(state, value);
    }

    template<>
    int get<int>(lua_State* state, int index) {
        int val = static_cast<int>(lua_tonumber(state, index));
        return val;
    }

    template<>
    float get<float>(lua_State* state, int index) {
        float val = lua_tonumber(state, index);
        return val;
    }

    template<>
    double get<double>(lua_State* state, int index) {
        double val = lua_tonumber(state, index);
        return val;
    }

    template<>
    lua_CFunction get<lua_CFunction>(lua_State* state, int index) {
        lua_CFunction val = lua_tocfunction(state, index);
        return val;
    }

    template<>
    std::string get<std::string>(lua_State* state, int index) {
        return lua_tostring(state, index);
    }
}