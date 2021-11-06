//
// Created by crax on 11/6/21.
//

#include "object/function.h"
#include "catch.hpp"

#include <iostream>
#include <cmath>

float my_pow(float base, float exp) {
    return std::pow(base, exp);
}

TEST_CASE("Function calling", "[functions]") {
    SECTION("Function creation though lambdas") {
        auto fun = create_callable(std::function([](int a, int b) {
            return a + b;
        }));

        auto result = fun->call(10, 20);
        REQUIRE(std::holds_alternative<function_call_result>(result));

        auto& result_optional = std::get<function_call_result>(result);
        REQUIRE(result_optional.has_value());
        REQUIRE(result_optional.value()->get<int>() == 30);
    }
    SECTION("Function creation through function pointers") {
        auto fun = create_callable(my_pow);
        auto result = fun->call(2.0f, 8.0f);
        REQUIRE(std::holds_alternative<function_call_result>(result));

        auto& result_optional = std::get<function_call_result>(result);
        REQUIRE(result_optional.has_value());
        REQUIRE(result_optional.value()->get<float>() == my_pow(2.0f, 8.0f));
    }
    SECTION("Function call with bad arguments") {
        auto fun = create_callable(my_pow);
        auto result = fun->call(false, "pippo");
        REQUIRE(std::holds_alternative<std::string>(result));

        auto error = std::get<std::string>(result);
        std::cout << "Error: " << error << "\n";
    }
}

TEST_CASE("Class method binding", "methods") {
    class foo {
    public:
        int x;
        [[nodiscard]] int bar(int a) const {
            return x + a;
        }

        static void say_hi() {
            std::cout << "Hello World\n";
        }
    };
    SECTION("Method call with bound object") {
        foo f{10};
        auto method = create_callable(&foo::bar);
        method->bind(&f);

        auto result = method->call(10);
        REQUIRE(std::holds_alternative<function_call_result>(result));

        auto& result_optional = std::get<function_call_result>(result);
        REQUIRE(result_optional.has_value());
        REQUIRE(result_optional.value()->get<int>() == 20);

    }
    SECTION("Method call without bound object") {
        foo f{10};
        auto method = create_callable(&foo::bar);

        auto result = method->call(10);
        REQUIRE(std::holds_alternative<std::string>(result));
    }
    SECTION("static method") {
        foo f{10};
        auto method = create_callable(&foo::say_hi);

        auto result = method->call(10);
        REQUIRE(std::holds_alternative<std::string>(result));
    }
}