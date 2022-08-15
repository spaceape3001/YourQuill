################################################################################
##
##  YOUR QUILL
##
################################################################################

include(${YOUR_QUILL_EXTERNAL}/asio.cmake)

add_executable(metavalues metavalues.cpp)
target_link_libraries(metavalues yqkernel yqhttp yqtoolbox)

add_executable(tcpsink tcpsink.cpp)
target_link_libraries(tcpsink asio)




