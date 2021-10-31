project(mikutpp C CXX)

set(GLM_HEADERS "${CMAKE_SOURCE_DIR}/thirdparty/glm/")

set(IMGUI_HEADERS ${CMAKE_SOURCE_DIR}/thirdparty/imgui ${CMAKE_SOURCE_DIR}/thirdparty/imgui/misc/cpp)
set(IMGUI_SOURCES 
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/backends/imgui_impl_sdl.cpp
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/backends/imgui_impl_opengl3.cpp
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/misc/cpp/imgui_stdlib.cpp
)

find_package(SDL2 REQUIRED)

add_library(thirdparty_imgui STATIC ${IMGUI_SOURCES})
target_include_directories(thirdparty_imgui PUBLIC ${IMGUI_HEADERS} ${SDL2_INCLUDE_DIRS})
target_link_libraries(thirdparty_imgui ${SDL2_LIBRARIES})

# file download
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/thirdparty/stb/stb/stb_image.h)
   file(DOWNLOAD "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h" ${CMAKE_SOURCE_DIR}/thirdparty/stb/stb/stb_image.h)
endif()
set(STB_HEADERS_DIR ${CMAKE_SOURCE_DIR}/thirdparty/stb/)

add_library(tinyfiledialogs ${CMAKE_SOURCE_DIR}/thirdparty/tinyfiledialogs/tinyfiledialogs.c)
target_include_directories(tinyfiledialogs PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/tinyfiledialogs/)

function(git_update_modules)
    execute_process(COMMAND git submodule update --recursive
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    COMMAND_ECHO STDOUT
                    )
endfunction()