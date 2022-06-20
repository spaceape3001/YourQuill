if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(asset_test_source ./test.cpp)
endif()

set(yq_asset_sources
    ${asset_test_source}
    
    ./asset.cpp
    ./NullCamera.ipp
    ./NullCamera.hpp
    ./SpaceCamera.ipp
    ./SpaceCamera.hpp
)

add_library(yqasset SHARED ${yq_asset_sources})
target_link_libraries(yqasset PUBLIC yqbasic yqmath yqengine)
LinkTest(yqasset)


