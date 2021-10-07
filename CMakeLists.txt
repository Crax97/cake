cmake_minimum_required(VERSION 3.15)
project(cake CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

include(thirdparty/CMakeLists.txt)

message(STATUS "Updating git dependencies")
git_update_modules()

add_subdirectory(modules/theather)
add_subdirectory(modules/renderer)

