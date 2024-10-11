################################################################################
##
##  YOUR QUILL
##
################################################################################

#macro(yq_test name)
#    add_executable(test_${name} ${name}.cpp)
#    target_link_libraries(test_${name} PRIVATE yq ${TBB_LIBRARIES} BoostUT)
#    add_test(NAME ${name} COMMAND test_${name} ${ARGN})
#endmacro()

#macro(engine_test name)
#    add_executable(test_${name} ${name}.cpp)
#    target_link_libraries(test_${name} PRIVATE yqengine ${TBB_LIBRARIES} BoostUT)
#    add_test(NAME ${name} COMMAND test_${name} ${ARGN})
#endmacro()

macro(doodletest name)
    add_executable(test_${name} ${name}.cpp)
    target_link_libraries(test_${name} PRIVATE yq_doodler BoostUT)
    add_test(NAME ${name} COMMAND test_${name} ${ARGN})
endmacro()

doodletest(b3parse)

macro(mithtest name)
    add_executable(test_${name} ${name}.cpp)
    target_link_libraries(test_${name} PRIVATE yq_mithril BoostUT)
    add_test(NAME ${name} COMMAND test_${name} ${ARGN})
endmacro()

mithtest(graphviz_builder)
