cmake_minimum_required(VERSION 3.15)
project(luanatic CXX)
set(CMAKE_CXX_STANDARD 20)

option(LUANATIC_BUILD_TESTS "Build luanatic tests?" ON)

find_package(Lua REQUIRED)

set(LUANATIC_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include/)
set(LUANATIC_SOURCES src/script.cpp src/utils.cpp)
add_library(luanatic STATIC ${LUANATIC_SOURCES})
target_include_directories(luanatic PUBLIC ${LUANATIC_INCLUDE_DIRS})
target_link_libraries(luanatic core loglib ${LUA_LIBRARIES})

set(LUANATIC_EXPORTED_DIRS ${LUANATIC_INCLUDE_DIRS} PARENT_SCOPE)

if(LUANATIC_BUILD_TESTS)
    add_executable(luanatic_tests
            tests/main.cpp
            tests/bindings.cpp
            tests/object_bindings.cpp
        )
    target_link_libraries(luanatic_tests luanatic)
    target_include_directories(luanatic_tests PRIVATE ${LUANATIC_INCLUDE_DIRS})
endif()