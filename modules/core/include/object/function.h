//
// Created by crax on 11/6/21.
//

#pragma once
#include "field.h"

#include <optional>
#include <variant>
#include <functional>

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

class field;
class function {
protected:
    using result_type = std::variant<std::string, std::optional<std::unique_ptr<bound_field>>>;
    std::vector<field*> m_parameters;
    std::optional<field*> m_return_type;
    virtual std::optional<std::unique_ptr<bound_field>> call_internal(const std::vector<std::unique_ptr<bound_field>>& fields) = 0;
public:

    template<typename... Args>
    result_type call(Args... args) {
        if(sizeof...(Args) < m_parameters.size()) {
            return "Size mismatch!";
        }
        std::vector<std::unique_ptr<bound_field>> parameters = pack_to_params(args...);
        auto in_param = parameters.begin();
        for(auto my_param = m_parameters.begin(); my_param != m_parameters.end(); my_param++, in_param++) {
            if(*my_param != (*in_param)->get_field_type()) {
                return "Different field types!";
            }
        }

        return call_internal(parameters);
    }
};

template<typename Arg>
Arg unpack_field(const std::vector<std::unique_ptr<bound_field>> &fields, size_t index) {
    return fields[index]->get<Arg>();
}

template<typename Return, typename... Args, size_t... I>
Return execute_call(const std::function<Return(Args...)>& fun, const std::vector<std::unique_ptr<bound_field>> &fields, std::index_sequence<I...>) {
    return fun(unpack_field<Args>(fields, I)...);
}

template<typename Return, typename... Args>
std::unique_ptr<function> make_function(std::function<Return(Args...)> the_function)  {
    class internal_function : public function {
    private:
        std::function<Return(Args...)> m_fun;
    protected:
        std::optional<std::unique_ptr<bound_field>>
        call_internal(const std::vector<std::unique_ptr<bound_field>> &fields) override {
            Return ret = execute_call(m_fun, fields, std::index_sequence_for<Args...>{});
            return bind(ret);
        }
    public:
        internal_function(const std::function<Return(Args...)>& fun)
            : m_fun(fun) { }
    };
    return std::make_unique<internal_function>(the_function);
}