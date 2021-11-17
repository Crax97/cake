//
// Created by crax on 11/16/21.
//

#pragma once

#include "lua_field.h"

namespace luanatic {
    class injector {
    public:
        virtual void inject(lua_State *state) = 0;

        virtual ~injector() = default;
    };

    template<typename Return, typename... Args>
    std::unique_ptr <injector> make_injector(Return (*function)(Args...)) {
        using fn_sig = Return(*)(Args...);
        class inner_injector : public injector {
        private:
            Return (*m_function)(Args...);

        public:
            explicit inner_injector(Return (*fun)(Args...)) : m_function(fun) {}

            void inject(lua_State *state) override {
                auto caller = [](lua_State *state) {
                    fn_sig function = *reinterpret_cast<fn_sig *>(lua_touserdata(state, 1));
                    return call_helper<Return, Args...>(function, state, std::index_sequence_for < Args... > {});
                };
                auto *address = reinterpret_cast<fn_sig *>(lua_newuserdata(state, sizeof(fn_sig)));
                *address = m_function;
                lua_newtable(state);
                lua_pushcfunction(state, +caller);
                lua_setfield(state, -2, "__call");
                lua_setmetatable(state, -2);
            }
        };
        return std::make_unique<inner_injector>(function);
    }

    template<typename T>
    std::unique_ptr <injector> make_field_injector(T *field, field_accessibility accessibility) {
        class inner_injector : public injector {
        private:
            T *m_field;
            field_accessibility m_acc;
        public:
            explicit inner_injector(T *f, field_accessibility a) : m_field(f), m_acc(a) {}

            void inject(lua_State *state) override {
                auto **field = reinterpret_cast<lua_field **>(lua_newuserdata(state, sizeof(lua_field * )));
                *field = make_field(m_field, m_acc);

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
}
