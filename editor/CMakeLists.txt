cmake_minimum_required(VERSION 3.15)
project(editor CXX)

set(EDITOR_SOURCE_FILES
${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor_application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/sidebar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/actor_editor.cpp
)
set(EDITOR_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(editor ${EDITOR_SOURCE_FILES})
target_include_directories(editor PRIVATE ${EDITOR_INCLUDE_DIRS} ${GLM_HEADERS} 
    ${GRAPHICSAPI_EXPORTED_DIRS}
    ${APPFRAMEWORK_EXPORTED_DIRS}
    ${GAMEFRAMEWORK_EXPORTED_DIRS}
    )
target_link_libraries(editor 
    graphics_api
    application
    gameframework
    core
    thirdparty_imgui
)