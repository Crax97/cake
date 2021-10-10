set(GLM_HEADERS "${CMAKE_SOURCE_DIR}/thirdparty/glm/")

set(IMGUI_HEADERS ${CMAKE_SOURCE_DIR}/thirdparty/imgui)
set(IMGUI_SOURCES 
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/backends/imgui_impl_sdl.cpp
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/backends/imgui_impl_opengl3.cpp
)

find_package(SDL2 REQUIRED)

add_library(thirdparty_imgui STATIC ${IMGUI_SOURCES})
target_include_directories(thirdparty_imgui PUBLIC ${IMGUI_HEADERS} ${SDL2_INCLUDE_DIRS})
target_link_libraries(thirdparty_imgui ${SDL2_LIBRARIES})

function(git_update_modules)
    execute_process(COMMAND git submodule update --recursive
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    COMMAND_ECHO STDOUT
                    )
endfunction()