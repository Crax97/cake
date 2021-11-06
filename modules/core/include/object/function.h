//
// Created by crax on 11/6/21.
//

#pragma once
#include "field.h"

#include <optional>
#include <variant>
#include <functional>

#include <iostream>

class bound_field {
protected:
    field* m_field{};

    virtual void* get_internal() = 0;
public:
    explicit bound_field(field* fi)
        : m_field(fi) { };

    [[nodiscard]] const field* get_field_type() const {
        return m_field;
    }

    template<typename T>
    T get() {
        return *static_cast<T*>(get_internal());
    };
};

template<typename T>
std::unique_ptr<bound_field> bind(const T& t) {
    class internal_field : public bound_field {
    private:
        T m_t;
    protected:
        void *get_internal() override {
            return &m_t;
        }
    public:
        explicit internal_field(const T& t) :
                bound_field(field_of<T>()), m_t(std::move(t)) { }
    };
    return std::make_unique<internal_field>(t);
}

template<typename ... Args>
std::vector<std::unique_ptr<bound_field> > pack_to_params(Args... args) {
    std::vector<std::unique_ptr<bound_field>> fields;
    fields.reserve(sizeof...(Args));
    int dummy[] = {0, (fields.emplace_back(std::move(bind(args))), 0)...};
    return fields;
}
template<typename ... Args>
std::vector<field*> build_args() {
    std::vector<field*> fields;
    fields.reserve(sizeof...(Args));
    int dummy[] = {0, (fields.emplace_back(field_of<Args>()), 0)...};
    return fields;
}

template<typename Arg>
Arg unpack_field(const std::vector<std::unique_ptr<bound_field>> &fields, size_t index) {
    return fields[index]->get<Arg>();
}

template<typename Return, typename... Args, size_t... I>
Return execute_call(const std::function<Return(Args...)>& fun, const std::vector<std::unique_ptr<bound_field>> &fields, std::index_sequence<I...>) {
    return fun(unpack_field<Args>(fields, I)...);
}

template<typename Return, typename Class, typename... Args, size_t... I>
Return execute_bound_call(Return (Class::*fun)(Args...), Class& instance, const std::vector<std::unique_ptr<bound_field>> &fields, std::index_sequence<I...>) {
    return (instance.*fun)(unpack_field<Args>(fields, I)...);
}

template<typename Return, typename Class, typename... Args, size_t... I>
Return execute_bound_call(Return (Class::*fun)(Args...) const, const Class& instance, const std::vector<std::unique_ptr<bound_field>> &fields, std::index_sequence<I...>) {
    return (instance.*fun)(unpack_field<Args>(fields, I)...);
}

using function_call_result = std::optional<std::unique_ptr<bound_field>>;

class callable {
protected:
    using result_type = std::variant<std::string, function_call_result>;
    std::vector<field*> m_parameters;
    std::optional<field*> m_return_type;
    virtual result_type call_internal(const std::vector<std::unique_ptr<bound_field>>& fields) = 0;
public:

    template<typename... Args>
    result_type call(Args... args) {
        if(sizeof...(Args) < m_parameters.size()) {
            return "Size mismatch!";
        }
        std::vector<std::unique_ptr<bound_field>> parameters = pack_to_params(args...);
        auto in_param = parameters.begin();
        for(auto& m_parameter : m_parameters) {
            const auto& in_param_type = m_parameter->get_field_descriptor()->get_typeinfo();
            const auto& this_param_type = (*in_param)->get_field_type()->get_field_descriptor()->get_typeinfo();
            if(in_param_type != this_param_type) {
                return "Different field types!";
            }
            in_param++;
        }
        return call_internal(parameters);
    }
};

class method : public callable {
protected:
    void* m_target {nullptr};
public:
    virtual void bind(void* target) {
        m_target = target;
    }

    template<typename Class>
    void bind(Class& instance) {
        bind(&instance);
    }
};

template<typename Return, typename... Args>
std::unique_ptr<callable> create_callable(const std::function<Return(Args...)>& the_function)  {
    class internal_function : public callable {
    private:
        std::function<Return(Args...)> m_fun;
    protected:
        result_type
        call_internal(const std::vector<std::unique_ptr<bound_field>> &fields) override {
            if constexpr(std::is_void_v<Return>) {
                execute_call(m_fun, fields, std::index_sequence_for<Args...>{});
                return {};
            } else {
                Return ret = execute_call(m_fun, fields, std::index_sequence_for<Args...>{});
                return bind(ret);
            }
        }
    public:
        explicit internal_function(const std::function<Return(Args...)>& fun)
                : m_fun(fun) {
            m_parameters = build_args<Args...>();
        }
    };
    return std::make_unique<internal_function>(the_function);
}

template<typename Return, typename Class, typename... Args>
std::unique_ptr<method> create_method(Return (Class::*method)(Args...))  {
    class internal_method : public method {
    private:
        Return (Class::*m_fun)(Args...);

        Class& instance() {
            MIKU_ASSERT(m_target && "No target bound to this object!");
            return *static_cast<Class*>(m_target);
        }

    protected:
        result_type
        call_internal(const std::vector<std::unique_ptr<bound_field>> &fields) override {
            if constexpr(std::is_void_v<Return>) {
                execute_bound_call(m_fun, instance(), fields, std::index_sequence_for<Args...>{});
                return {};
            } else {
                Return ret = execute_bound_call(m_fun, instance(), fields, std::index_sequence_for<Args...>{});
                return bind(ret);
            }
        }
    public:
        explicit internal_method(Return (Class::*fun)(Args...))
                : m_fun(fun) {
                    m_parameters = build_args<Args...>();
                }
    };
    return std::make_unique<internal_method>(method);
}

template<typename Return, typename Class, typename... Args>
std::unique_ptr<method> create_method(Return (Class::*method )(Args...) const)  {
    class internal_method : public method {
    private:
        Return (Class::*m_fun)(Args...) const;

        const Class& instance() {
            MIKU_ASSERT(m_target && "No target bound to this object!");
            return *static_cast<const Class*>(m_target);
        }

    protected:
        result_type
        call_internal(const std::vector<std::unique_ptr<bound_field>> &fields) override {
            if(!m_target) {
                return "No object is bound to this method";
            }
            if constexpr(std::is_void_v<Return>) {
                execute_bound_call(m_fun, instance(), fields, std::index_sequence_for<Args...>{});
                return {};
            } else {
                Return ret = execute_bound_call(m_fun, instance(), fields, std::index_sequence_for<Args...>{});
                return ::bind(ret);
            }
        }
    public:
        explicit internal_method(Return (Class::*fun)(Args...)  const)
                : m_fun(fun) { }
    };
    return std::make_unique<internal_method>(method);
}

template<typename Fun>
std::unique_ptr<callable> create_callable(const Fun& fun) {
    return create_callable(std::function(fun));
}
