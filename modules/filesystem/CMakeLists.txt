cmake_minimum_required(VERSION 3.15)
project(filesystem CXX)

set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_CXX_STANDARD 20)

set(FILESYSTEM_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(FILESYSTEM_SOURCES
    src/utils.cpp
)

set(FILESYSTEM_HEADERS_DIRS ${FILESYSTEM_HEADERS_DIR} PARENT_SCOPE)

add_library(filesystem STATIC ${FILESYSTEM_SOURCES})
target_include_directories(filesystem PUBLIC include/ )
