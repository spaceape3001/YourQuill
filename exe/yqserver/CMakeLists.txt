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
target_link_libraries(yqserver yqtoolbox yqkernel yqhttp asio )


