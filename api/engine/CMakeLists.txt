################################################################################
##
##  YOUR QUILL
##
################################################################################

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(engine_test_source ./test.cpp)
endif()

set(yq_engine_sources
    ${engine_test_source}

    ./engine.cpp

    ./Vulqan.cpp

    ./asset/Asset.cpp
    ./asset/AssetCache.cpp
    
    ./gui/ImWidget.cpp
    ./gui/ImWindow.cpp
    ./gui/StdEdits.cpp
    
    ./render/Pipeline.cpp
    ./render/Rendered.cpp
    ./render/Render3D.cpp

    ./shader/Shader.cpp

    ./texture/Texture.cpp

#    ./vulqan/VqCommandFrame.cpp
    ./vulqan/VqInternal.cpp
#    ./vulqan/VqObject.cpp
#    ./vulqan/VqObjectFrame.cpp
    ./vulqan/VqPipeline.cpp
)

add_library(yqengine SHARED ${yq_engine_sources})
target_compile_definitions(yqengine PRIVATE VK_ENABLE_BETA_EXTENSIONS=1)
target_link_libraries(yqengine PUBLIC yqbasic yqconfig yqmath ${Vulkan_LIBRARIES} ${GLFW3_LIBRARY} imgui stb vma)
LinkTest(yqengine)


