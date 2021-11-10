//
// Created by crax on 11/9/21.
//

#pragma once

#include <type_traits>
#include <utility>

#include "utils.h"

namespace luanatic {
    template<typename Class>
    class binder {
        const std::string m_name{};

        lua_CFunction m_constructor = nullptr;
        std::unordered_map<std::string, lua_CFunction> m_functions;
        std::unordered_map<std::string, lua_CFunction> m_getters;
    public:
        explicit binder(std::string  name)
            : m_name(std::move(name)) { }

        template<typename... Args>
        binder& with_constructor() {
            m_constructor = [](lua_State* state) {
                void* data = lua_newuserdata(state, sizeof(Class));
                new(data) Class(luanatic::get<Args>(state)...);
                return 1;
            };

            return *this;
        }

        template<typename Return, typename... Args>
        binder& with_method(const std::string& method_name, Return (Class::*method)(Args...)) {

            return *this;
        }
        template<typename Return, typename... Args>
        binder& with_method(const std::string& method_name, Return (Class::*method)(Args...) const) {

            return *this;
        }
        template<typename Return, typename... Args>
        binder& with_associated_function(const std::string& function_name, Return (function)(Class*, Args...)) {

            return *this;
        }
        template<typename PropType>
        binder& with_property(const std::string& property_name, PropType Class::*prop) {

            return *this;
        }
    };
}
