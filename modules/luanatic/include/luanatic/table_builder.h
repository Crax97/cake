//
// Created by crax on 11/14/21.
//

#pragma once

#include "lua_field.h"
#include "injector.h"
#include "utils.h"

#include <memory>
#include <unordered_map>
#include <string>

namespace luanatic {
    class table_builder {
    private:
        std::unordered_map<std::string, std::unique_ptr<injector>> m_function_injectors {};
        std::unordered_map<std::string, std::unique_ptr<injector>> m_field_injectors {};

    public:
        table_builder() = default;
        template<typename Return, typename... Args>
        table_builder&& with_function(const std::string& function_name, Return (*function)(Args...)) {
            std::unique_ptr<injector> injector = make_injector(function);
            m_function_injectors.insert({function_name, std::move(injector)});
            return std::move(*this);
        }

        template<typename Type>
        table_builder&& with_field(const std::string& field_name, Type* field_ptr, field_accessibility accessibility = field_accessibility::read_and_write) {
            std::unique_ptr<injector> injector = make_field_injector(field_ptr, accessibility);
            m_field_injectors.insert({field_name, std::move(injector)});
            return std::move(*this);
        }

        void inject(std::string_view name, lua_State* state) {

            auto field_get = [](lua_State* state) {
                auto field_name = get<std::string>(state, -1);
                lua_getmetatable(state, -2);
                if(lua_getfield(state, -1, field_name.c_str()) == 0) {
                    lua_pop(state, 1);
                    luaL_error(state, "Could not find field");
                    return 0;
                }
                if(void* data = luaL_testudata(state, -1, get_metatable_name<lua_field>())) {
                    lua_field* field = *reinterpret_cast<lua_field**>(data);
                    lua_pop(state, 1);
                    field->get(nullptr, state);
                }
                lua_pop(state, 1);
                return 1;
            };

            auto field_set = [](lua_State* state) {
                auto field_name = get<std::string>(state, -2);
                lua_getmetatable(state, -3);
                if(lua_getfield(state, -1, field_name.c_str()) != 0) {
                    if (void *data = luaL_testudata(state, -1, get_metatable_name<lua_field>())) {
                        lua_field *field = *reinterpret_cast<lua_field **>(data);
                        lua_pop(state, 2);
                        field->set(nullptr, state);
                    }
                } else {
                    lua_pop(state, 1);
                    lua_rawset(state, -3);
                }
                return 0;
            };
            lua_newtable(state);

            for(auto& fun : m_function_injectors) {
                fun.second->inject(state);
                lua_setfield(state, -2, fun.first.c_str());
            }

            lua_newtable(state);
            push(state, +field_get);
            lua_setfield(state, -2, "__index");
            push(state, +field_set);
            lua_setfield(state, -2, "__newindex");
            for(auto& prop: m_field_injectors) {
                prop.second->inject(state);
                luaL_newmetatable(state, get_metatable_name<lua_field>());
                lua_setmetatable(state, -2);
                lua_setfield(state, -2, prop.first.c_str());
            }
            lua_setmetatable(state, -2);
            lua_setglobal(state, name.data());
        }
    };
}