cmake_minimum_required(VERSION 3.23.0)

if("${PROJECT_NAME}" STREQUAL "")
    project(YourQuill VERSION 0.0.0.3)
endif()

if(NOT YOUR_QUILL_KERNEL)
    message(FATAL_ERROR "Currently this must be a submodule to the project, and the kernel already added!")
endif()

include(configure.cmake)

add_subdirectory(3rd)
add_subdirectory(api)
add_subdirectory(test)
add_subdirectory(tool)
add_subdirectory(plugin)
add_subdirectory(exe)
