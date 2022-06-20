################################################################################
##
##  YOUR QUILL
##
################################################################################

add_executable(yquill 
    yquill.cpp 
    yquill.hpp 
    yscanner.cpp
    yserver.cpp
)
target_link_libraries(yquill yq )
target_include_directories(yquill PRIVATE .)

