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

macro(basic_test name)
    add_executable(test_${name} ${name}.cpp)
    target_link_libraries(test_${name} PRIVATE yqbasic ${TBB_LIBRARIES})
    add_test(NAME ${name} COMMAND test_${name} ${ARGN})
endmacro()

macro(math_test name)
    add_executable(test_${name} ${name}.cpp)
    target_link_libraries(test_${name} PRIVATE yqmath ${TBB_LIBRARIES})
    add_test(NAME ${name} COMMAND test_${name} ${ARGN})
endmacro()

macro(engine_test name)
    add_executable(test_${name} ${name}.cpp)
    target_link_libraries(test_${name} PRIVATE yqengine ${TBB_LIBRARIES})
    add_test(NAME ${name} COMMAND test_${name} ${ARGN})
endmacro()

macro(physics_test name)
    add_executable(test_${name} ${name}.cpp)
    target_link_libraries(test_${name} PRIVATE yqphysics ${TBB_LIBRARIES})
    add_test(NAME ${name} COMMAND test_${name} ${ARGN})
endmacro()

macro(earth_test name)
    add_executable(test_${name} ${name}.cpp)
    target_link_libraries(test_${name} PRIVATE yqearth ${TBB_LIBRARIES})
    add_test(NAME ${name} COMMAND test_${name} ${ARGN})
endmacro()

#add_subdirectory(fraction)

basic_test(encode64)
basic_test(http)
basic_test(strings)
basic_test(refs)
basic_test(variants)

math_test(fraction)
math_test(units)
math_test(vector1)
math_test(vector2)
math_test(vector3)
math_test(vector4)

earth_test(earthdates)


