//
// Created by crax on 11/14/21.
//

#pragma once

#include "lua_field.h"
#include "utils.h"
#include "injector.h"

#include <memory>
#include <unordered_map>
#include <string>

namespace luanatic {
    namespace {
        template<typename Class, typename Return, typename... Args, std::size_t... I>
        int do_call(lua_State *state, Class *instance, Return (Class::*fun)(Args...), std::index_sequence<I...>) {
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
        int do_call(lua_State *state, Class *instance, Return (Class::*fun)(Args...) const, std::index_sequence<I...>) {
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
        Class* constructor_helper(lua_State *state, std::index_sequence<I...>) {
            int arg_size = static_cast<int>(sizeof...(Args));
            return new Class(luanatic::get<Args>(state, -(arg_size - I + 1))...);
        }
        lua_field* to_lua_field(lua_State* state, int field_index = -1) {
            auto* field = luaL_testudata(state, field_index, get_metatable_name<lua_field>());
            return field ? *reinterpret_cast<lua_field**>(field) : nullptr;
        };
    }

    struct lua_object_info {
        bool is_created_in_lua{};
        bool was_destroyed = false;
    };
    using indexing_function = std::function<bool(lua_State*, std::string_view)>;

    bool index_noop(lua_State*, std::string_view) {
        return false;
    }
    struct class_info {
        indexing_function set = &index_noop;
        indexing_function get = &index_noop;

        class_info()
            : get(&index_noop), set(&index_noop) {}
    };

    int try_call_custom_setter(lua_State* state, std::string_view field_name) {
        if(luaL_getmetafield(state, 1, "custom_setter")) {
            auto* custom_setter = *reinterpret_cast<const indexing_function**>(lua_touserdata(state, -1));
            lua_pop(state, 1);
            return (*custom_setter)(state, field_name);
        }
        luaL_error(state, "could not set field %s on %s", field_name.data(), lua_tostring(state, 1));
        return 0;
    };
    int try_call_custom_getter(lua_State* state, std::string_view field_name) {
        if(luaL_getmetafield(state, 1, "custom_getter")) {
            auto* custom_getter = *reinterpret_cast<const indexing_function**>(lua_touserdata(state, -1));
            lua_pop(state, 1);
            return (*custom_getter)(state, field_name);
        }
        luaL_error(state, "could not get field %s on %s", field_name.data(), lua_tostring(state, 1));
        return 0;
    };

    template<typename Class, typename... Args>
    int constructor(lua_State* state) {
        auto** instance_address = reinterpret_cast<Class**>(lua_newuserdata(state, sizeof(Class**)));
        *instance_address = constructor_helper<Class, Args...>(state, std::index_sequence_for<Args...>{});

        // 1. Bind correct metatable
        luaL_newmetatable(state, get_metatable_name<Class>());
        lua_setmetatable(state, -2);

        // 2. Configure data for garbage collector
        auto* instance_info = reinterpret_cast<lua_object_info*>(lua_newuserdata(state, sizeof(lua_object_info)));
        instance_info->is_created_in_lua = true;
        lua_setuservalue(state, -2);


        return 1;
    }

    template<typename Class, typename Return, typename... Args>
    std::unique_ptr <injector> make_method_injector(Return (Class::*function)(Args...)) {
        using fn_sig = Return(Class::*)(Args...);
        class inner_injector : public injector {
        private:
            fn_sig m_method;

        public:
            explicit inner_injector(fn_sig method) : m_method(method) {}

            void inject(lua_State *state) override {
                auto caller = [](lua_State *state) {
                    int self_addr = lua_gettop(state) - sizeof...(Args);
                    auto* instance = get_self<Class>(state, self_addr);
                    fn_sig function = *reinterpret_cast<fn_sig *>(lua_touserdata(state, 1));
                    return do_call( state, instance, function, std::index_sequence_for < Args... > {});
                };
                auto *address = reinterpret_cast<fn_sig *>(lua_newuserdata(state, sizeof(fn_sig)));
                *address = m_method;
                lua_newtable(state);
                lua_pushcfunction(state, +caller);
                lua_setfield(state, -2, "__call");
                lua_setmetatable(state, -2);
            }
        };
        return std::make_unique<inner_injector>(function);
    }

    template<typename Class, typename Return, typename... Args>
    std::unique_ptr <injector> make_method_injector(Return (Class::*function)(Args...) const) {
        using fn_sig = Return(Class::*)(Args...) const;
        class inner_injector : public injector {
        private:
            fn_sig m_method;

        public:
            explicit inner_injector(fn_sig method) : m_method(method) {}

            void inject(lua_State *state) override {
                auto caller = [](lua_State *state) {
                    int self_addr = lua_gettop(state) - sizeof...(Args);
                    auto* instance = get_self<Class>(state, self_addr);
                    fn_sig function = *reinterpret_cast<fn_sig *>(luaL_testudata(state, 1, get_metatable_name<fn_sig>()));
                    return do_call( state, instance, function, std::index_sequence_for < Args... > {});
                };
                auto *address = reinterpret_cast<fn_sig *>(lua_newuserdata(state, sizeof(fn_sig)));
                *address = m_method;

                luaL_newmetatable(state, get_metatable_name<fn_sig>());
                lua_pushcfunction(state, +caller);
                lua_setfield(state, -2, "__call");
                lua_setmetatable(state, -2);
            }
        };
        return std::make_unique<inner_injector>(function);
    }

    template<typename T, typename Class>
    std::unique_ptr <injector> make_class_field_injector(T Class::*field, field_accessibility accessibility) {
        class inner_injector : public injector {
        private:
            T Class::*m_field;
            field_accessibility m_acc;
        public:
            explicit inner_injector(T Class::*f, field_accessibility a) : m_field(f), m_acc(a) {}

            void inject(lua_State *state) override {
                auto **field = reinterpret_cast<lua_field **>(lua_newuserdata(state, sizeof(lua_field * )));
                *field = make_field_for_class(m_field, m_acc);

                lua_newtable(state);
                auto gc = [](lua_State *state) {
                    auto **field = reinterpret_cast<lua_field **>(lua_touserdata(state, -1));
                    delete *field;
                    return 0;
                };
                push(state, +gc);
                lua_setfield(state, -2, "__gc");
                lua_setmetatable(state, -2);
            }
        };
        return std::make_unique<inner_injector>(field, accessibility);
    }

    template<typename Class>
    class class_builder {
    private:
        std::unordered_map<std::string, std::unique_ptr<injector>> m_function_injectors {};
        std::unordered_map<std::string, std::unique_ptr<injector>> m_field_injectors {};

        class_info infos;

    public:
        class_builder() = default;

        template<typename Return, typename... Args>
        class_builder&& with_function(const std::string& function_name, Return (*function)(Args...)) {
            std::unique_ptr<injector> injector = make_injector(function);
            m_function_injectors.insert({function_name, std::move(injector)});
            return std::move(*this);
        }

        template<typename Return, typename... Args>
        class_builder&& with_method(const std::string& function_name, Return (Class::*function)(Args...)) {
            std::unique_ptr<injector> injector = make_method_injector(function);
            m_function_injectors.insert({function_name, std::move(injector)});
            return std::move(*this);
        }

        template<typename Return, typename... Args>
        class_builder&& with_method(const std::string& function_name, Return (Class::*function)(Args...) const) {
            std::unique_ptr<injector> injector = make_method_injector(function);
            m_function_injectors.insert({function_name, std::move(injector)});
            return std::move(*this);
        }

        template<typename Type>
        class_builder&& with_field(const std::string& field_name, Type* field_ptr, field_accessibility accessibility = field_accessibility::read_and_write) {
            std::unique_ptr<injector> injector = make_field_injector(field_ptr, accessibility);
            m_field_injectors.insert({field_name, std::move(injector)});
            return std::move(*this);
        }

        template<typename Type>
        class_builder&& with_class_field(const std::string& field_name,Type Class::* field_ptr, field_accessibility accessibility = field_accessibility::read_and_write) {
            std::unique_ptr<injector> injector = make_class_field_injector(field_ptr, accessibility);
            m_field_injectors.insert({field_name, std::move(injector)});
            return std::move(*this);
        }

        class_builder<Class>&& with_custom_getter(const indexing_function& fn) {
            infos.get = fn;
            return std::move(*this);
        }

        class_builder<Class>&& with_custom_setter(const indexing_function& fn) {
            infos.set = fn;
            return std::move(*this);
        }

        void inject(lua_State* state) {
            auto field_get = [](lua_State* state) {
                auto* instance = get_self<Class>(state);
                auto field_name = get<std::string>(state, 2);
                lua_getmetatable(state, 1);
                if(lua_getfield(state, -1, field_name.c_str()) == 0) {
                    lua_pop(state, 1);
                    lua_getfield(state, -1, "class_info");
                    auto* my_infos = reinterpret_cast<class_info*>(lua_touserdata(state, -1));
                    lua_pop(state, 3);
                    if(!my_infos->get(state, field_name)) {
                        lua_pushnil(state);
                    }
                    return 1;
                }
                if(void* data = luaL_testudata(state, -1, get_metatable_name<lua_field>())) {
                    lua_field* field = *reinterpret_cast<lua_field**>(data);
                    lua_pop(state, 3);
                    field->get(instance, state);
                } else {
                    lua_replace(state, -2);
                }
                return 1;
            };

            auto field_set = [](lua_State* state) {
                auto* instance = get_self<Class>(state);
                auto field_name = get<std::string>(state, 2);
                lua_getmetatable(state, 1);
                if(lua_getfield(state, -1, field_name.c_str()) != 0) {
                    if (void *data = luaL_testudata(state, -1, get_metatable_name<lua_field>())) {
                        lua_field *field = *reinterpret_cast<lua_field **>(data);
                        lua_pop(state, 2);
                        field->set(instance, state);
                    }
                } else {
                    lua_pop(state, 1);
                    lua_getfield(state, -1, "class_info");
                    auto* my_infos = reinterpret_cast<class_info*>(lua_touserdata(state, -1));
                    lua_pop(state, 2);
                    if(!my_infos->set(state, field_name)) {
                        lua_rawset(state, -3);
                    }
                }
                return 0;
            };

            auto gc = [](lua_State* state) {
                auto* instance = get_self<Class>(state);
                lua_getuservalue(state, 1);
                auto* info = reinterpret_cast<lua_object_info*>(lua_touserdata(state, -1));
                lua_pop(state, 1);
                if(info->is_created_in_lua && !info->was_destroyed) {
                    instance->~Class();
                    delete instance;
                    info->was_destroyed = true;
                }
                return 0;
            };
            luaL_newmetatable(state, get_metatable_name<Class>());
            push(state, +field_get);
            lua_setfield(state, -2, "__index");
            push(state, +field_set);
            lua_setfield(state, -2, "__newindex");
            push(state, +gc);
            lua_setfield(state, -2, "__gc");

            for(auto& fun : m_function_injectors) {
                fun.second->inject(state);
                lua_setfield(state, -2, fun.first.c_str());
            }
            for(auto& prop: m_field_injectors) {
                prop.second->inject(state);
                luaL_newmetatable(state, get_metatable_name<lua_field>());
                lua_setmetatable(state, -2);
                lua_setfield(state, -2, prop.first.c_str());
            }

            auto* info = reinterpret_cast<class_info*>(lua_newuserdata(state, sizeof(class_info)));
            new (info) class_info();
            *info = infos;
            lua_setfield(state, -2, "class_info");

            lua_setglobal(state, get_metatable_name<Class>());
        }


        void bind(std::string_view name, Class* instance, lua_State* state) {
            auto** instance_address = reinterpret_cast<Class**>(lua_newuserdata(state, sizeof(Class**)));
            *instance_address = instance;

            // 1. Bind correct metatable
            luaL_newmetatable(state, get_metatable_name<Class>());
            lua_setmetatable(state, -2);

            // 2. Configure data for garbage collector
            auto* instance_info = reinterpret_cast<lua_object_info*>(lua_newuserdata(state, sizeof(lua_object_info)));
            instance_info->is_created_in_lua = false;
            lua_setuservalue(state, -2);

            lua_setglobal(state, name.data());
        }
    };
}