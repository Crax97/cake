#include "catch.hpp"

#include "luanatic/script.h"
#include "luanatic/binder.h"
#include "luanatic/utils.h"

class MyTestClass {

public:
    float z;
    int x;
    void inst_say_z() {
        std::cout << "Z is " << z << "\n";
    }
    void say_x() const {
        std::cout << "Hello World!\n";
        std::cout << "X is " << x << "\n";
    }

    int sum_with_x(int param) const {
        return param + x;
    }

    void add_to_x(int off) {
        x += off;
    }
};

TEST_CASE("Testing object bindings", "Object")  {
    SECTION("Testing method calling") {
        auto src = R"(
    function test()
        obj = MyTestClass(10.0,  12)
        obj:add_to_x(15)
        return obj
    end
)";
        auto script = luanatic::script::compile_source(src);
        auto test_binder = luanatic::binder<MyTestClass>("MyTestClass", script->get_state())
                .with_constructor<float, int>()
                .with_method("add_to_x", &MyTestClass::add_to_x);
        REQUIRE(script.has_value());
        auto call_result = script->call<MyTestClass*>("test");
        REQUIRE(call_result.has_value());
        REQUIRE(call_result != nullptr);
        REQUIRE(call_result.value()->x == 27);
    }
    SECTION("Testing const method calling + return") {
        auto src = R"(
    function test()
        obj = MyTestClass(10.0,  12)
        return obj:sum_with_x(16)
    end
)";
        auto script = luanatic::script::compile_source(src);
        auto test_binder = luanatic::binder<MyTestClass>("MyTestClass", script->get_state())
                .with_constructor<float, int>()
                .with_method("sum_with_x", &MyTestClass::sum_with_x);
        REQUIRE(script.has_value());
        auto call_result = script->call<int>("test");
        REQUIRE(call_result.has_value());
        REQUIRE(call_result.value() == 28);
    }

    SECTION("Additional getter/setter") {
        auto src = R"(
    function test()
        x = obj.param_from_custom_indexer
        obj.param_to_custom_indexer = 84
        return x
    end
    )";
        int number;
        auto script = luanatic::script::compile_source(src);
        MyTestClass instance {};
        auto test_binder = luanatic::binder<MyTestClass>("MyTestClass", script->get_state())
                .with_constructor<float, int>()
                .with_method("sum_with_x", &MyTestClass::sum_with_x)
                .with_custom_getter([](lua_State* state, std::string_view what) {
                    return 0;
                })
                .with_custom_setter([&number](lua_State* state, std::string_view what){
                    return 0;
                });
        REQUIRE(script.has_value());
        auto call_result = script->call<int>("test");
        REQUIRE(call_result.has_value());
        REQUIRE(call_result.value() == 42);
        REQUIRE(number == 84);

    }

    SECTION("Everything together") {
        auto src = R"(
    function test()
        obj = MyTestClass(10.0,  12)
        obj:add_to_x(10)
        obj:add_to_x(13)
        sum = obj:sum_with(x)
        obj:say_x()
        value = obj:do_foo()
        obj.z = obj.z + 156
        return obj
    end
)";
        auto script = luanatic::script::compile_source(src);
        auto test_binder = luanatic::binder<MyTestClass>("MyTestClass", script->get_state())
                .with_constructor<float, int>()
                .with_method("add_to_x", &MyTestClass::add_to_x)
                .with_method("sum_with", &MyTestClass::sum_with_x)
                .with_method("say_x", &MyTestClass::say_x)
                .with_associated_function("do_foo", +[](lua_State *state) {
                    std::cout << " I am doing foo\n";
                    return 0;
                })
                .with_property("z", &MyTestClass::z);
        REQUIRE(script.has_value());
        auto call_result = script->call<MyTestClass *>("test");
        REQUIRE(call_result.has_value());
        REQUIRE(call_result != nullptr);
        REQUIRE(call_result.value()->z == 166.0f);
        REQUIRE(call_result.value()->x == 35);
    }
}