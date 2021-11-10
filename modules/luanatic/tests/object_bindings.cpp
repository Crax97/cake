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
    auto test_binder = luanatic::binder<MyTestClass>("MyTestClass")
            .with_constructor<float, int>()
            .with_method("add_to_x", &MyTestClass::add_to_x)
            .with_method("say_x", &MyTestClass::say_x)
            .with_method("sum_with_x", &MyTestClass::sum_with_x)
            .with_property("z", &MyTestClass::z);

    auto src = R"(
    function test()
        obj = MyTestClass(10.0,  12)
        obj.add_to_x(10)
        obj.add_to_x(13)
        sum = obj.sum_with(x)
        obj.say_x()
        value = obj.do_foo()
        obj.z = obj.z + 156
        return obj
    end
)";
    lua_State* state = luaL_newstate();

    auto script = luanatic::script::compile_source(src);
    REQUIRE(script.has_value());
    auto call_result = script->call<MyTestClass*>("test");
    REQUIRE(call_result != nullptr);
    REQUIRE(call_result->z == 166.0f);
    REQUIRE(call_result->x == 35);
}