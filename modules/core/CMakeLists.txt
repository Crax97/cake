cmake_minimum_required(VERSION 3.15)
project(gameframework CXX)
set(CMAKE_CXX_STANDARD 20)

set(CORE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CORE_SOURCES 
    src/field.cpp
    src/std_extensions.cpp
)
add_library(core STATIC ${CORE_SOURCES})
target_include_directories(core PUBLIC ${CORE_INCLUDE_DIRS} ${GLM_HEADERS})

set(CORE_EXPORTED_DIRS ${CORE_INCLUDE_DIRS} PARENT_SCOPE)
