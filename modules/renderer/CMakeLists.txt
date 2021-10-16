cmake_minimum_required(VERSION 3.15)
project(renderer CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(RENDERER_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(RENDERER_SOURCES 
${CMAKE_CURRENT_SOURCE_DIR}/src/renderer/renderer.cpp
)
add_library(renderer STATIC ${RENDERER_SOURCES})
target_include_directories(renderer PUBLIC 
    ${RENDERER_INCLUDE_DIRS}
    ${GRAPHICSAPI_EXPORTED_DIRS}
    ${STB_HEADERS_DIR}
    ${GLM_HEADERS}
)
target_link_libraries(renderer graphics_api)

set(RENDERER_EXPORTED_DIRS ${RENDERER_EXPORTED_DIRS} PARENT_SCOPE)
