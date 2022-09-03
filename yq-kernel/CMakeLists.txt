cmake_minimum_required(VERSION 3.23.0)

if("${PROJECT_NAME}" STREQUAL "")
    project(YourQuill VERSION 0.0.0.3)
endif()

if(NOT YOUR_QUILL_TOOLBOX)
    message(FATAL_ERROR "Currently this must be a submodule to the project, and the toolbox already added!")
endif()

include(configure.cmake)

add_subdirectory(api)
add_subdirectory(test)
add_subdirectory(tool)
