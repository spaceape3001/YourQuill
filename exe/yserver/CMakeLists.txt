################################################################################
##
##  YOUR QUILL
##
################################################################################

project(yserver)

add_executable(${PROJECT_NAME}
    yquill.cpp 
    yquill.hpp 
    yscanner.cpp
    yserver.cpp
)
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        yq_mithril 
        yq_asio 
        yq_math
)

#add_library(yqupdate SHARED
#    update/update.cpp
#)

#target_include_directories(yqupdate 
#    PUBLIC 
#        ${CMAKE_CURRENT_LIST_DIR} 
#    PRIVATE
#        ${ImageMagick_INCLUDE_DIRS}
#)

#target_link_libraries(yqupdate 
#    PUBLIC 
#        mithril
#        ${ImageMagick_LIBRARIES}
#)
#LinkTest(yqupdate)


SET(PLUGIN_DIR ${CMAKE_BINARY_DIR}/bin/plugin/${PROJECT_NAME})
SET(CMAKE_SHARED_LIBRARY_PREFIX "")
add_subdirectory(plugins)
