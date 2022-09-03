################################################################################
##
##  YOUR QUILL
##
################################################################################

add_executable(kvtree kvtree.cpp)
target_link_libraries(kvtree yqkernel)

add_executable(metavalues metavalues.cpp)
target_link_libraries(metavalues yqkernel yqhttp yqtoolbox)

add_executable(tcpsink tcpsink.cpp)
target_link_libraries(tcpsink asio)

add_executable(wkspinfo wkspinfo.cpp)
target_link_libraries(wkspinfo yqkernel)


