################################################################################
##
##  YOUR QUILL
##
################################################################################

add_executable(kvtree kvtree.cpp)
target_link_libraries(kvtree yqkernel)

add_executable(wkspinfo wkspinfo.cpp)
target_link_libraries(wkspinfo yqkernel)



