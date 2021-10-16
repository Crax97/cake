cmake_minimum_required(VERSION 3.15)
project(application CXX)
set(CMAKE_CXX_STANDARD 20)

set(APPLICATION_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include/)
set(APPLICATION_SOURCES 
    src/application.cpp
)
set(APPFRAMEWORK_EXPORTED_DIRS ${APPLICATION_INCLUDE_DIRS} PARENT_SCOPE)

add_library(application ${APPLICATION_SOURCES})
target_link_libraries(application graphics_api inputsystem renderer)
target_include_directories(application PUBLIC 
    ${GRAPHICSAPI_EXPORTED_DIRS}
    ${RENDERER_INCLUDE_DIRS}
    ${INPUTSYSTEM_EXPORTED_DIRS}
    ${APPLICATION_INCLUDE_DIRS}
)