cmake_minimum_required(VERSION 3.23.0)

if("${PROJECT_NAME}" STREQUAL "")
    project(YourQuil VERSION 0.0.0.3)
    
    #   TODO.... add in verbage
    
    include(${YQ_UTILTIY_DIR}/YourQuill.cmake)
endif()

add_subdirectory(3rd)
add_subdirectory(api)
add_subdirectory(test)
add_subdirectory(tool)
add_subdirectory(sdk)

