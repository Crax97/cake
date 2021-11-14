//
// Created by crax on 11/14/21.
//

#pragma once

#include "lua_field.h"
#include "utils.h"

#include <memory>
#include <unordered_map>
#include <string>

namespace luanatic {

    class function_injector {
    public:
        virtual void inject(lua_State* state) = 0;
        virtual ~function_injector() = default;
    };

    template<typename Return, typename... Args>
    std::unique_ptr<function_injector> make_injector(Return (*function)(Args...)) {
        using fn_sig = Return(*)(Args...);
        class inner_injector : public function_injector {
        private:
            Return (*m_function)(Args...);
        public:
            explicit inner_injector(Return (*fun)(Args...)) : m_function(fun) {}
            void inject(lua_State* state) override {
                auto caller = [](lua_State* state) {
                    int fn_index = -static_cast<int>(sizeof...(Args) + 1);
                    fn_sig function = *reinterpret_cast<fn_sig*>(lua_touserdata(state, fn_index));
                    return call_helper(function, state, std::index_sequence_for<Args...>{});
                };
                auto* address = reinterpret_cast<fn_sig*>(lua_newuserdata(state, sizeof(fn_sig)));
                *address = m_function;
                lua_newtable(state);
                lua_pushcfunction(state, +caller);
                lua_setfield(state, -2, "__call");
                lua_setmetatable(state, -2);
            }
        };
        return std::make_unique<inner_injector>(function);
    }

    class table_builder {
    private:
        std::unordered_map<std::string, std::unique_ptr<function_injector>> m_function_injectors {};

    public:
        table_builder() = default;
        template<typename Return, typename... Args>
        table_builder&& with_function(const std::string& function_name, Return (*function)(Args...)) {
            std::unique_ptr<function_injector> injector = make_injector(function);
            m_function_injectors.insert({function_name, std::move(injector)});
            return std::move(*this);
        }

        template<typename Type>
        table_builder&& with_field(const std::string& field_name, Type* field_ptr, field_accessibility accessibility = field_accessibility::read_and_write) {

            return std::move(*this);
        }

        void inject(std::string_view name, lua_State* state) {
            lua_newtable(state);
            for(const auto& func: m_function_injectors) {
                func.second->inject(state);
                lua_setfield(state, -2, func.first.data());
            }

            lua_setglobal(state, name.data());
        }
    };
}