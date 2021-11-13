//
// Created by crax on 11/7/21.
//

#include "catch.hpp"

#include "luanatic/script.h"

#include <iostream>

int divide(int a, int b) {
    return a / b;
}

float divide_float(float a, float b) {
    return a / b;
}

TEST_CASE("Testing functions", "calling") {
    auto source = R"(
function answer()
    return 42
end

function divide(a, b)
    return a / b
end
)";

    auto script = luanatic::script::compile_source(source);
    REQUIRE(script.has_value());

    SECTION("answer() == 42") {
        auto value = script->call<int>("answer");
        REQUIRE(value == 42);
    }
    SECTION("divide(4,2) == 2") {
        auto value = script->call<float>("divide", 4, 2);
        REQUIRE(value == 2);
    }
}
TEST_CASE("Binding functions", "binding") {
    auto source = R"(
function call1()
    return divide(4, 2)
end

function call2(a, b)
    return divide_float(6, 2)
end
)";

    auto script = luanatic::script::compile_source(source);
    REQUIRE(script.has_value());
    script->bind("divide", &divide);
    script->bind("divide_float", &divide_float);

    SECTION("divide(4, 2) == 2") {
        auto value = script->call<int>("call1");
        REQUIRE(value == 2);
    }
    SECTION("divide_float(6, 2) == 3") {
        auto value = script->call<float>("call2");
        REQUIRE(value == 3);
    }
}