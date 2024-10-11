################################################################################
##
##  YOUR QUILL
##
################################################################################

add_executable(metavalues metavalues.cpp)
target_link_libraries(metavalues yq_mithril)

add_executable(wkspinfo wkspinfo.cpp)
target_link_libraries(wkspinfo yq_mithril)


