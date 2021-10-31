cmake_minimum_required(VERSION 3.15)
project(loglib CXX)
set(CMAKE_CXX_STANDARD 20)

set(LOGLIB_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include/)
set(LOGLIB_SOURCES src/logging/logger.cpp)
add_library(loglib STATIC ${LOGLIB_SOURCES})
target_include_directories(loglib PUBLIC ${LOGLIB_INCLUDE_DIRS})

set(LOGLIB_EXPORTED_DIRS ${LOGLIB_INCLUDE_DIRS} PARENT_SCOPE)
