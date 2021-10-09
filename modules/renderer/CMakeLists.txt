cmake_minimum_required(VERSION 3.18)
project(renderer CXX C)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(CMAKE_BUILD_TYPE Debug)

option(BUILD_EXAMPLES "Wether to build the examples of the project" OFF)

find_package(OpenGL)
find_package(SDL2 REQUIRED)

set (OPENGL_SOURCES
        src/opengl/opengl_mesh.cpp
        src/opengl/opengl_instanced_mesh.cpp
        src/opengl/opengl_texture.cpp
        src/opengl/opengl_shader.cpp
        src/opengl/opengl_framebuffer.cpp
        src/opengl/opengl_graphics_api.cpp
    src/opengl/glad.c
)

set (SOURCES
        src/api_factory.cpp
        src/window.cpp
)

if (OPENGL_FOUND)
    add_compile_definitions(OPENGL_FOUND)
    set(SOURCES ${SOURCES} ${OPENGL_SOURCES})
    set(OPTIONAL_INCLUDES src/opengl/)

    if(NOT MSVC)
        set(OPTIONAL_LINK_LIBS dl)
    endif()
endif()

set(RENDERER_EXPORTED_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

add_library(renderer STATIC ${SOURCES})
target_include_directories(renderer PUBLIC include ${SDL2_INCLUDE_DIRS})
target_include_directories(renderer PRIVATE ${OPTIONAL_INCLUDES})

target_link_libraries(renderer ${OPTIONAL_LINK_LIBS} ${SDL2_LIBRARIES})

if (BUILD_EXAMPLES)
    file(GLOB EXAMPLES examples/*)
    
    foreach(EXAMPLE_DIR ${EXAMPLES})
        get_filename_component(EXAMPLE_NAME ${EXAMPLE_DIR} NAME_WE)
        if(${EXAMPLE_NAME} MATCHES "commons" OR ${EXAMPLE_NAME} MATCHES "assets")
            continue()
        endif()
        message ("Building example ${EXAMPLE_NAME}...")
        file(GLOB EXAMPLE_SOURCES ${EXAMPLE_DIR}/* )
        
        add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCES})
        target_include_directories(${EXAMPLE_NAME} PRIVATE ${EXAMPLE_DIR} ${GLM_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/examples/commons/)
        target_link_libraries(${EXAMPLE_NAME} renderer)
    endforeach()

    file(COPY "examples/assets" DESTINATION ${CMAKE_BINARY_DIR})
endif()

