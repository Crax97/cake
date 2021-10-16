cmake_minimum_required(VERSION 3.15)
project(spectacle CXX)

set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_CXX_STANDARD 20)

set(SPECTACLE_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SPECTACLE_SOURCES 
    src/stage.cpp
    src/actor.cpp
    src/camera.cpp
    src/transform.cpp
)

if(!${GLM_HEADERS})
    message(FATAL_ERROR "No GLM Headers found!")
endif()

set(SPECTACLE_EXPORTED_DIRS ${SPECTACLE_HEADERS_DIR} ${PROPERTYSYSTEM_EXPORTED_DIRS} PARENT_SCOPE)

add_library(spectacle STATIC ${SPECTACLE_SOURCES})
target_link_libraries(spectacle property_system)
target_include_directories(spectacle PUBLIC include/ ${GLM_HEADERS})
