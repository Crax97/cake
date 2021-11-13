//
// Created by crax on 11/9/21.
//

#pragma once

#include <type_traits>
#include <typeinfo>
#include <utility>

#include "utils.h"
#include "lua_field.h"

namespace luanatic {
    template<typename Class, typename Return, typename... Args, std::size_t... I>
    int do_call(lua_State* state, Class* instance, Return (Class::*fun)(Args...), std::index_sequence<I...>) {
        int arg_size = static_cast<int>(sizeof...(Args));
        if constexpr(std::is_void_v<Return>) {
            (instance->*fun)(get<Args>(state, -(arg_size - I))...);
            return 0;
        } else {
            auto ret = (instance->*fun)(get<Args>(state, -(arg_size - I))...);
            push(state, ret);
            return 1;
        }
    }
    template<typename Class, typename Return, typename... Args, std::size_t... I>
    int do_call(lua_State* state, Class* instance, Return (Class::*fun)(Args...) const, std::index_sequence<I...>) {
        int arg_size = static_cast<int>(sizeof...(Args));
        if constexpr(std::is_void_v<Return>) {
            (instance->*fun)(get<Args>(state, -(arg_size - I))...);
            return 0;
        } else {
            auto ret = (instance->*fun)(get<Args>(state, -(arg_size - I))...);
            push(state, ret);
            return 1;
        }
    }
    template<typename Class, typename... Args, std::size_t... I>
    void constructor_helper(void* addr, lua_State* state, std::index_sequence<I...>) {
        int arg_size = static_cast<int>(sizeof...(Args));
        std::tuple<Args...> debug = {luanatic::get<Args>(state, -(arg_size - I + 1))...};
        new (addr) Class(luanatic::get<Args>(state, -(arg_size - I + 1))...);
    }

    template<typename Class>
    class binder {
        const std::string m_name{};

        lua_State* m_state;
    public:
        explicit binder(std::string name, lua_State* state)
            : m_name(std::move(name)), m_state(state) {
            luaL_newmetatable(m_state, get_metatable_name<Class>());
            auto index = [](lua_State* state){
                auto field_name = get<std::string>(state);
                luaL_getmetafield(state, -2, field_name.c_str());
                if(luaL_testudata(state, -1, get_metatable_name<lua_field>())) {
                    void* field_ptr = luaL_checkudata(state, -1, get_metatable_name<lua_field>());
                    auto* field = *reinterpret_cast<lua_field**>(field_ptr);
                    lua_pop(state, 1);
                    auto* base = lua_touserdata(state, -2);
                    field->get(base, state);
                }
                return 1;
            };

            auto set_index = [](lua_State* state) {
                auto field_name = get<std::string>(state, -2);
                luaL_getmetafield(state, -3, field_name.c_str());
                if(luaL_testudata(state, -1, get_metatable_name<lua_field>())) {
                    void* field_ptr = luaL_checkudata(state, -1, get_metatable_name<lua_field>());
                    auto* field = *reinterpret_cast<lua_field**>(field_ptr);
                    lua_pop(state, 1);
                    auto* base = lua_touserdata(state, -3);
                    field->set(base, state);
                }
                return 0;

            };
            push(state, +index);
            lua_setfield(state, -2, "__index");
            push(state, +set_index);
            lua_setfield(state, -2, "__newindex");
            lua_setglobal(m_state, get_metatable_name<Class>());
        }

        template<typename... Args>
        binder& with_constructor() {
            auto constructor = [](lua_State* state) {
                void* address = lua_newuserdata(state, sizeof(Class));
                constructor_helper<Class, Args...>(address, state, std::index_sequence_for<Args...>{});
                luaL_setmetatable(state, get_metatable_name<Class>());
                return 1;
            };
            push(m_state, +constructor);
            lua_setglobal(m_state, m_name.c_str());
            return *this;
        }

        template<typename Return, typename... Args>
        binder& with_method(const std::string& method_name, Return (Class::*method)(Args...)) {
            using function_type = Return (Class::*)(Args...);

            lua_getglobal(m_state, get_metatable_name<Class>());
            const static auto method_field_name = "original_method";
            auto* data_store = reinterpret_cast<function_type*>(lua_newuserdata(m_state, sizeof(function_type)));
            *data_store = method;
            auto fun = [](lua_State* state) {
                int last_arg = -static_cast<int>(sizeof...(Args));
                auto* instance = reinterpret_cast<Class*>(lua_touserdata(state, last_arg - 1));
                auto function = *reinterpret_cast<function_type*>(lua_touserdata(state, last_arg - 2));
                return do_call(state, instance, function, std::index_sequence_for<Args...>{});
            };

            lua_newtable(m_state);
            lua_pushcfunction(m_state, fun);
            lua_setfield(m_state, -2, "__call");
            lua_setmetatable(m_state, -2);
            lua_setfield(m_state, -2, method_name.c_str());
            lua_pop(m_state, 1);
            return *this;
        }
        template<typename Return, typename... Args>
        binder& with_method(const std::string& method_name, Return (Class::*method)(Args...) const) {
            using function_type = Return (Class::*)(Args...) const;

            lua_getglobal(m_state, get_metatable_name<Class>());
            const static auto method_field_name = "original_method";
            auto* data_store = reinterpret_cast<function_type*>(lua_newuserdata(m_state, sizeof(function_type)));
            *data_store = method;
            auto fun = [](lua_State* state) {
                int last_arg = -static_cast<int>(sizeof...(Args));
                auto* instance = reinterpret_cast<Class*>(lua_touserdata(state, last_arg - 1));
                auto function = *reinterpret_cast<function_type*>(lua_touserdata(state, last_arg - 2));
                return do_call(state, instance, function, std::index_sequence_for<Args...>{});
            };

            lua_newtable(m_state);
            lua_pushcfunction(m_state, fun);
            lua_setfield(m_state, -2, "__call");
            lua_setmetatable(m_state, -2);
            lua_setfield(m_state, -2, method_name.c_str());
            lua_pop(m_state, 1);
            return *this;
        }

        binder& with_associated_function(const std::string& function_name, lua_CFunction function) {
            luaL_getmetatable(m_state, get_metatable_name<Class>());
            push(m_state, function);
            lua_setfield(m_state, -2, function_name.c_str());
            lua_pop(m_state, 1); // Remove the metatable
            return *this;
        }
        binder& with_custom_getter(const std::function<int(lua_State*, std::string_view)>& getter) {
            return *this;
        }
        binder& with_custom_setter(const std::function<int(lua_State*, std::string_view)>& setter) {
            return *this;
        }
        template<typename PropType>
        binder& with_property(const std::string& property_name, PropType Class::*prop, field_accessibility accessibility = field_accessibility::read_and_write) {
            using prop_signature = PropType Class::*;

            lua_getglobal(m_state, get_metatable_name<Class>());

            auto* getter_storage = reinterpret_cast<lua_field**>(lua_newuserdata(m_state, sizeof(lua_field*)));
            *getter_storage = make_field_for_class(prop, accessibility);

            luaL_newmetatable(m_state, get_metatable_name<lua_field>());
            lua_setmetatable(m_state, -2);

            lua_setfield(m_state, -2, property_name.c_str());
            return *this;
        }
    };
}
