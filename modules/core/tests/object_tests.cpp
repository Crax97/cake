//
// Created by crax on 11/7/21.
//

#include "catch.hpp"

#include "object/object.h"
#include "object/macros.h"

class TestClass : public object {
    CORE_GENERATE_BODY(TestClass)
private:
    int a;
    float b;
    char c;
public:
    double x{};
    TestClass(int aa, float bb, char cc)
        : a(aa), b(bb), c(cc) { }

    [[nodiscard]] int sum_with_a(int num) const {
        return num + a;
    }
};

CORE_BEGIN_DESCRIPTOR(TestClass)
        CORE_REFLECT_MEMBER(a)
        CORE_REFLECT_MEMBER(b)
        CORE_REFLECT_MEMBER(c)
        CORE_REFLECT_MEMBER(x)
        CORE_REFLECT_METHOD(sum_with_a)
CORE_END_DESCRIPTOR()

CORE_IMPLEMENT_FUNCTIONS(TestClass)

TEST_CASE("Object properties", "[functions]") {
    SECTION("Check that getting a field's value is correct") {
        TestClass instance {1, 2.0f, '3'};
        instance.x = 10.0;
        {
            auto field = instance.get_descriptor()->find_field("x");
            REQUIRE(field.has_value());
            REQUIRE(field.value()->get<double>(&instance) == 10.0);
        }
        {
            auto field = instance.get_descriptor()->find_field("a");
            REQUIRE(field.has_value());
            REQUIRE(field.value()->get<int>(&instance) == 1);
        }
    }
    SECTION("Check that setting a field's value is correct") {
        TestClass instance {1, 2.0f, '3'};
        instance.x = 10.0;
        {
            auto field = instance.get_descriptor()->find_field("x");
            REQUIRE(field.has_value());
            field.value()->set(&instance, 123.0);
            REQUIRE(instance.x == 123.0);
        }
    }
    SECTION("Check that calling a method yields the correct value") {
        TestClass instance {1, 2.0f, '3'};
        instance.x = 10.0;
        {
            auto method = instance.get_descriptor()->find_method("sum_with_a");
            REQUIRE(method.has_value());
            method.value()->bind(&instance);
            auto result_maybe = method.value()->call(10);
            REQUIRE(std::holds_alternative<function_call_result>(result_maybe));
            auto& call_result = std::get<function_call_result>(result_maybe);
            REQUIRE(call_result.has_value());
            REQUIRE(call_result.value()->get<int>() == 11);
        }
    }
}