#include "catch.hpp"

#include "luanatic/script.h"
#include "luanatic/binder.h"
#include "luanatic/table_builder.h"
#include "luanatic/utils.h"

class MyTestClass {

public:
    float z;
    int x;
    void inst_say_z() const {
        std::cout << "Z is " << z << "\n";
    }
    void say_x() const {
        std::cout << "Hello World!\n";
        std::cout << "X is " << x << "\n";
    }

    [[nodiscard]] int sum_with_x(int param) const {
        return param + x;
    }

    void add_to_x(int off) {
        x += off;
    }
};

TEST_CASE("Testing table building") {
    SECTION("Functions") {
        auto summer = [](int a, float b) {
            return static_cast<float>(a) + b;
        };
        auto builder = luanatic::table_builder()
                .with_function("summer", +summer);
        auto src = R"(
    function test()
        return Functions.summer(10, 20);
    end
)";

        auto compiled_script = luanatic::script::compile_source(src);
        REQUIRE(compiled_script.has_value());
        auto& script = compiled_script.value();
        builder.inject("Functions", script->get_state());
        auto result = script->call<float>("test");
        REQUIRE(result.has_value());
        REQUIRE(result.value() == 30.0f);
    }
    SECTION("Fields") {
        int number = 100;
        auto builder = luanatic::table_builder()
                .with_field("x", &number);
        auto src = R"(
    function test()
        Functions.x = 30;
    end
)";
        auto compiled_script = luanatic::script::compile_source(src);
        REQUIRE(compiled_script.has_value());
        auto& script = compiled_script.value();
        builder.inject("Functions", script->get_state());
        auto result = script->call<float>("test");
        REQUIRE(result.has_value());
        REQUIRE(number == 30.0f);
    }

    SECTION("Read only fields") {
        int number = 100;
        auto builder = luanatic::table_builder()
                .with_field("x", &number, luanatic::field_accessibility::read);
        auto src = R"(
    function test()
        Functions.x = 30;
    end
)";
        auto compiled_script = luanatic::script::compile_source(src);
        REQUIRE(compiled_script.has_value());
        auto& script = compiled_script.value();
        script->set_on_error_function([](lua_State* state) {
            SUCCEED();
        });
        builder.inject("Functions", script->get_state());
        auto result = script->call<float>("test");
        REQUIRE(!result.has_value());
    }
}

TEST_CASE("Testing object bindings", "Object")  {
    SECTION("Testing method calling") {
        auto src = R"(
    function test()
        obj = MyTestClass(10.0,  12)
        obj:add_to_x(15)
        return obj
    end
)";
        auto compiled_script = luanatic::script::compile_source(src);
        auto test_binder = luanatic::class_builder<MyTestClass>()
                .with_method("add_to_x", &MyTestClass::add_to_x);

        REQUIRE(compiled_script.has_value());
        auto& script = compiled_script.value();
        test_binder.inject(script->get_state());
        luanatic::bind(script->get_state(), "MyTestClass", &luanatic::constructor<MyTestClass, float, int>);

        auto call_result = script->call<MyTestClass**>("test");
        REQUIRE(call_result.has_value());
        REQUIRE(call_result != nullptr);
        REQUIRE((*call_result.value())->x == 27);
    }
    SECTION("Testing const method calling + return") {
        auto src = R"(
    function test()
        obj = MyTestClass(10.0,  12)
        return obj:sum_with_x(16)
    end
)";
        auto compiled_script = luanatic::script::compile_source(src);
        auto test_binder = luanatic::class_builder<MyTestClass>()
                .with_method("sum_with_x", &MyTestClass::sum_with_x);

        REQUIRE(compiled_script.has_value());
        auto& script = compiled_script.value();
        luanatic::bind(script->get_state(), "MyTestClass", &luanatic::constructor<MyTestClass, float, int>);
        test_binder.inject(script->get_state());
        auto call_result = script->call<int>("test");
        REQUIRE(call_result.has_value());
        REQUIRE(call_result.value() == 28);
    }

    SECTION("Testing accessing non existent field") {
        auto src = R"(
    function test()
        obj = MyTestClass(10.0,  12)
        return obj.booba
    end
)";
        auto test_binder = luanatic::class_builder<MyTestClass>();

        auto compiled_script = luanatic::script::compile_source(src);
        REQUIRE(compiled_script.has_value());
        auto& script = compiled_script.value();
        script->set_on_error_function(+[](lua_State* state) {
            SUCCEED();
        });
        luanatic::bind(script->get_state(), "MyTestClass", &luanatic::constructor<MyTestClass, float, int>);
        test_binder.inject(script->get_state());
        auto call_result = script->call<int>("test");
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
        auto compiled_script = luanatic::script::compile_source(src);
        REQUIRE(compiled_script.has_value());
        auto& script = compiled_script.value();
        MyTestClass instance {};
        auto test_binder = luanatic::class_builder<MyTestClass>()
                .with_method("sum_with_x", &MyTestClass::sum_with_x)
                .with_custom_getter([](lua_State* state, std::string_view what) {
                    luanatic::push(state, what);
                    return true;
                })
                .with_custom_setter([&number](lua_State* state, std::string_view what){
                    if(what == "param_to_custom_indexer") {
                        number = luanatic::get<int>(state);
                        return true;
                    }
                    return false;
                });
        test_binder.inject(script->get_state());

        test_binder.bind("obj", &instance, script->get_state());
        auto call_result = script->call<std::string>("test");
        REQUIRE(call_result.has_value());
        REQUIRE(call_result.value() == "param_from_custom_indexer");
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
        auto test_binder = luanatic::class_builder<MyTestClass>()
                .with_method("sum_with_x", &MyTestClass::sum_with_x)
                .with_method("add_to_x", &MyTestClass::add_to_x)
                .with_method("sum_with", &MyTestClass::sum_with_x)
                .with_method("say_x", &MyTestClass::say_x)
                .with_function("do_foo", +[]() {
                    std::cout << " I am doing foo\n";
                    return 0;
                })
                .with_class_field("z", &MyTestClass::z);

        auto compiled_script = luanatic::script::compile_source(src);
        REQUIRE(compiled_script.has_value());
        auto& script = compiled_script.value();
        script->set_on_error_function(&luanatic::print_stack);
        luanatic::bind(script->get_state(), "MyTestClass", &luanatic::constructor<MyTestClass, float, int>);
        test_binder.inject(script->get_state());

        auto call_result = script->call<MyTestClass **>("test");
        REQUIRE(call_result.has_value());
        REQUIRE(call_result != nullptr);
        REQUIRE((*call_result.value())->z == 166.0f);
        REQUIRE((*call_result.value())->x == 35);
    }
}