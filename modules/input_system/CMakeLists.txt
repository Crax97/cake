cmake_minimum_required(VERSION 3.15)
project(input_system CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)

set(INPUTSYSTEM_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(INPUTSYSTEM_SOURCES 
    src/inputsystem.cpp
)
set(INPUTSYSTEM_EXPORTED_DIRS ${INPUTSYSTEM_INCLUDE_DIRS} PARENT_SCOPE)

add_library(inputsystem ${INPUTSYSTEM_SOURCES})
target_link_libraries(inputsystem ${SDL2_LIBRARIES})
target_include_directories(inputsystem PUBLIC ${INPUTSYSTEM_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})