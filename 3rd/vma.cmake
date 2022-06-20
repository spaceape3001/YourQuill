################################################################################
##
##  YOUR QUILL
##
################################################################################

add_library(vma SHARED 
    vma.cpp
)

target_include_directories(vma PUBLIC ./vma/include)
target_link_libraries(vma PUBLIC ${Vulkan_LIBRARIES})
target_compile_options(vma PRIVATE -w) 
   
LinkTest(vma)


