cmake_minimum_required(VERSION 3.00)
project(Theather CXX)

set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_CXX_STANDARD 20)

file(GLOB HEADERS include/*)

set(SOURCES 
    src/stage.cpp
    src/actor.cpp
    src/camera.cpp
    src/transform.cpp
)

if(!${GLM_HEADERS})
    message(FATAL_ERROR "No GLM Headers found!")
endif()

add_library(spectacle STATIC ${SOURCES} ${HEADERS})
target_include_directories(spectacle PUBLIC include/ ${GLM_HEADERS})
