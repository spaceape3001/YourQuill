################################################################################
##
##  YOUR QUILL
##
################################################################################

project(yq_mithrilVk)


add_library(${PROJECT_NAME} SHARED
    mithrilVk.cpp
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        yq_mithril
        yq_tachyon
)

LinkTest(${PROJECT_NAME})

