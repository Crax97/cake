cmake_minimum_required(VERSION 3.15)
project(gameframework CXX)
set(CMAKE_CXX_STANDARD 20)

set(GAMEFRAMEWORK_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(GAMEFRAMEWORK_SOURCES 
${CMAKE_CURRENT_SOURCE_DIR}/src/rendering_world.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/components/component_repository.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/components/rendering_component.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/components/sprite_component.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/loaders/texture_loader.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/game.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/actor.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/camera.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/stage.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/transform.cpp
)
add_library(gameframework STATIC ${GAMEFRAMEWORK_SOURCES})
target_include_directories(gameframework PUBLIC 
${GAMEFRAMEWORK_INCLUDE_DIRS} 
    ${APPFRAMEWORK_INCLUDE_DIRS} 
    ${GRAPHICSAPI_EXPORTED_DIRS}
    ${RENDERER_EXPORTED_DIRS} 
    ${STB_HEADERS_DIR}
)
target_link_libraries(gameframework graphics_api renderer application core)

set(GAMEFRAMEWORK_EXPORTED_DIRS ${GAMEFRAMEWORK_INCLUDE_DIRS} PARENT_SCOPE)
