macro(yq_test name)
    add_executable(test_${name} ${name}.cpp)
    target_link_libraries(test_${name} PRIVATE yq ${TBB_LIBRARIES})
    add_test(NAME ${name} COMMAND test_${name} ${ARGN})
endmacro()

macro(mt_math name)
    add_executable(test_${name} ${name}.cpp)
    target_link_libraries(test_${name} PRIVATE  yq ${TBB_LIBRARIES})
endmacro()
macro(mathtest name)
    mt_math(${name})
    add_test(NAME ${name} COMMAND test_${name} ${ARGN})
endmacro()
macro(mt_util name)
    add_executable(test_${name} ${name}.cpp)
    target_link_libraries(test_${name} PRIVATE yq ${TBB_LIBRARIES})
endmacro()
macro(utiltest name)
    mt_util(${name})
    add_test(NAME ${name} COMMAND test_${name} ${ARGN})
endmacro()


#add_subdirectory(attr)
#add_subdirectory(db)
#add_subdirectory(fraction)

yq_test(encode64)
yq_test(http)
yq_test(strings)
yq_test(variants)



