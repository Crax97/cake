cmake_minimum_required(VERSION 3.15)
project(property_system CXX)

set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 17)

set(PROPERTYSYSTEM_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(PROPERTYSYSTEM_SOURCES 
    src/property.cpp
)

if(!${GLM_HEADERS})
    message(FATAL_ERROR "No GLM Headers found!")
endif()

set(PROPERTYSYSTEM_EXPORTED_DIRS ${PROPERTYSYSTEM_HEADERS_DIR} PARENT_SCOPE)

add_library(property_system STATIC ${PROPERTYSYSTEM_SOURCES})
target_include_directories(property_system PUBLIC include/ ${GLM_HEADERS})
