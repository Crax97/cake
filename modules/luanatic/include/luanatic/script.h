//
// Created by crax on 11/7/21.
//

#pragma once
#include "lua.hpp"
#include "utils.h"

#include <iostream>
#include <string_view>
#include <optional>
#include <tuple>
#include <memory>
#include <functional>

namespace luanatic {
    class script {
    struct private_tag {};
        lua_State* m_state;
    private:
        std::function<void(lua_State*)> m_on_error_function;
        void log_error() const;
        template<typename... Args>
        int internal_call(std::string_view function_name, Args... args) {
            lua_getglobal(m_state, function_name.data());
            int _dummy[] = {0, (push(m_state, args), 0)...};
            return lua_pcall(m_state, sizeof...(Args), 1, 0);
        }
    public:

        explicit script(lua_State* state, private_tag)
                : m_state(state) { }
        ~script() {
            if(m_state) {
                lua_close(m_state);
            }
        }

        static std::optional<std::unique_ptr<script>> compile_source(std::string_view source) {
            lua_State* state = luaL_newstate();
            int result = luaL_dostring(state, source.data());
            if(result != LUA_OK) {
                lua_close(state);
                return {};
            }
            return std::make_unique<script>(state, private_tag{});
        }

        void set_on_error_function(const std::function<void(lua_State*)>& function) {
            m_on_error_function = function;
        }

        lua_State* get_state() {
            return m_state;
        }

        template<typename... Args>
        void call(std::string_view function_name, Args... args) {
            internal_call(function_name, args...);
        }

        template<typename... Return, typename... Args>
        std::tuple<Return...> call(std::string_view function_name, Args... args, std::enable_if<1 < sizeof...(Return)>) {
            internal_call(function_name, args...);
            std::tuple<Return...> ret = {pop<Return>(m_state)...};
            return ret;
        }

        template<typename Return, typename... Args>
        std::optional<Return> call(std::string_view function_name, Args... args) {
            if (internal_call(function_name, args...) != LUA_OK) {
                log_error();
                if(m_on_error_function) {
                    m_on_error_function(m_state);
                }
                return { };
            }
            if constexpr(std::is_pointer_v<Return>) {
                return pop_pointer<Return>(m_state);
            } else {
                return pop<Return>(m_state);
            }
        }

        template<typename Return, typename... Args>
        void bind(std::string_view name, Return (*fun)(Args...)) {
            using function_type = Return(*)(Args...);
            auto caller = [](lua_State* state) {
                int idx = -static_cast<int>(sizeof...(Args)) - 1;

                auto* actual = *reinterpret_cast<function_type*>(lua_touserdata(state, idx));
                return call_helper<Return, Args...>(actual, state, std::index_sequence_for<Args...>());
            };
            auto* lua_fun = reinterpret_cast<function_type*>(lua_newuserdata(m_state, sizeof(function_type)));
            *lua_fun = fun;

            lua_newtable(m_state);
            push(m_state, static_cast<lua_CFunction>(caller));
            lua_setfield(m_state, -2, "__call");
            lua_setmetatable(m_state, -2);

            lua_setglobal(m_state, name.data());
        }
    };
}