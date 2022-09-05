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
    page/page.cpp
    update/update.cpp
)
target_link_libraries(yqserver yqtoolbox yqkernel yqhttp asio )
target_include_directories(yqserver PRIVATE .)

