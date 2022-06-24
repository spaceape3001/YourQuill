################################################################################
##
##  YOUR QUILL
##
################################################################################

macro(yq_test name)
    add_executable(test_${name} ${name}.cpp)
    target_link_libraries(test_${name} PRIVATE yq ${TBB_LIBRARIES})
    add_test(NAME ${name} COMMAND test_${name} ${ARGN})
endmacro()

#add_subdirectory(fraction)

yq_test(encode64)
yq_test(fraction)
yq_test(http)
yq_test(strings)
yq_test(refs)
yq_test(units)
yq_test(variants)
yq_test(vector1)
yq_test(vector2)
yq_test(vector3)
yq_test(vector4)


