cmake_minimum_required(VERSION 3.15)
project(cake CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

option(BUILD_EDITOR "Wether to build the example library" ON)

include(thirdparty/thirdparty.cmake)

message(STATUS "Updating git dependencies")
git_update_modules()

include_directories(generated_headers)

add_subdirectory(modules/loglib)
add_subdirectory(modules/core)
add_subdirectory(modules/luanatic)
add_subdirectory(modules/game_framework)
add_subdirectory(modules/graphics_api)
add_subdirectory(modules/renderer)
add_subdirectory(modules/input_system)
add_subdirectory(modules/application)

if(BUILD_EDITOR)
    add_subdirectory(editor)
endif()
