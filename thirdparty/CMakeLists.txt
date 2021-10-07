set(GLM_HEADERS "${CMAKE_SOURCE_DIR}/thirdparty/glm/")

function(git_update_modules)
    execute_process(COMMAND git submodule update --recursive
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    COMMAND_ECHO STDOUT
                    )
endfunction()