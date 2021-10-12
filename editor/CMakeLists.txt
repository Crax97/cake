cmake_minimum_required(VERSION 3.15)
project(editor CXX)

file(GLOB EDITOR_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/* )
set(EDITOR_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(editor ${EDITOR_SOURCE_FILES})
target_include_directories(editor PRIVATE ${EDITOR_INCLUDE_DIRS} ${GLM_HEADERS} 
    ${RENDERER_EXPORTED_DIRS}
    ${APPFRAMEWORK_EXPORTED_DIRS}
    ${GAMEFRAMEWORK_EXPORTED_DIRS}
    ${SPECTACLE_EXPORTED_DIRS}
    )
target_link_libraries(editor 
    renderer
    application
    spectacle
    gameframework
    thirdparty_imgui
)