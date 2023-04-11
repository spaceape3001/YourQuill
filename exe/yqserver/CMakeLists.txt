################################################################################
##
##  YOUR QUILL
##
################################################################################

add_executable(yqserver
    yquill.cpp 
    yquill.hpp 
    yscanner.cpp
    yserver.cpp
)
target_link_libraries(yqserver yqtoolbox mithril asio )

SET(PLUGIN_DIR ${CMAKE_BINARY_DIR}/bin/plugin/yqserver)
SET(CMAKE_SHARED_LIBRARY_PREFIX "")
add_subdirectory(plugins)
