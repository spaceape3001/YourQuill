################################################################################
##
##  YOUR QUILL
##
################################################################################

add_executable(kvtree kvtree.cpp)
target_link_libraries(kvtree mithril)

add_executable(metavalues metavalues.cpp)
target_link_libraries(metavalues mithril yqtoolbox)

add_executable(tcpsink tcpsink.cpp)
target_link_libraries(tcpsink asio)

add_executable(wkspinfo wkspinfo.cpp)
target_link_libraries(wkspinfo mithril)


