cmake_minimum_required(VERSION 3.15)
project(gameframework CXX)
set(CMAKE_CXX_STANDARD 20)

option(CORE_BUILD_TESTS "Wether to build core tests" ON)

set(CORE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CORE_SOURCES
        src/field.cpp
        src/object.cpp
        src/descriptor.cpp
        src/resource.cpp
    src/std_extensions.cpp
        include/object/function.h src/function.cpp)
add_library(core STATIC ${CORE_SOURCES})
target_include_directories(core PUBLIC ${CORE_INCLUDE_DIRS} ${GLM_HEADERS})
target_link_libraries(core loglib)

set(CORE_EXPORTED_DIRS ${CORE_INCLUDE_DIRS} PARENT_SCOPE)

if(CORE_BUILD_TESTS)
    add_executable(core_tests tests/main.cpp tests/function_tests.cpp tests/object_tests.cpp)
    target_link_libraries(core_tests PRIVATE core)
endif()