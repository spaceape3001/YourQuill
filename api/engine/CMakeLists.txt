if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(engine_test_source ./test.cpp)
endif()

set(yq_engine_sources
    ${engine_test_source}
    
    ./asset/Asset.cpp
    ./asset/Asset.hpp
    ./asset/AssetCache.hpp
    ./asset/AssetCompiler.hpp
    ./asset/AssetLoader.hpp
    ./asset/AssetInfoWriter.hpp

    ./render/ColorComponent.hpp    
    ./render/CullMode.hpp    
    ./render/FrontFace.hpp    
    ./render/LogicOp.hpp    
    ./render/PipelineConfig.hpp    
    ./render/PolygonMode.hpp
    ./render/Renderable.cpp
    ./render/Renderable.hpp
    ./render/StencilOp.hpp
    ./render/Topology.hpp
    
    ./shader/Shader.cpp
    ./shader/Shader.hpp
    ./shader/ShaderCache.hpp
    ./shader/ShaderCompiler.hpp
    ./shader/ShaderLoader.hpp
    ./shader/ShaderInfoWriter.hpp
    
    ./texture/Texture.cpp
    ./texture/Texture.hpp
    ./texture/TextureCache.hpp
    ./texture/TextureLoader.hpp
    ./texture/TextureInfoWriter.hpp
    
    ./ui/ImWindow.hpp
    ./ui/ImWindow.cpp
    
    ./vulqan/VqApp.cpp
    ./vulqan/VqApp.cpp
    ./vulqan/VqEnums.cpp
    ./vulqan/VqFence.cpp
    ./vulqan/VqFence.hpp
    ./vulqan/VqFencePool.cpp
    ./vulqan/VqFencePool.hpp
    ./vulqan/VqMonitor.cpp
    ./vulqan/VqMonitor.hpp
    ./vulqan/VqShaderStages.cpp
    ./vulqan/VqShaderStages.hpp
    ./vulqan/VqStructs.hpp
    ./vulqan/VqUtils.cpp
    ./vulqan/VqUtils.hpp
    ./vulqan/VqWindow.cpp
    ./vulqan/VqWindow.hpp
)

add_library(yqengine SHARED ${yq_engine_sources})
target_link_libraries(yqengine PUBLIC yqbasic yqconfig ${Vulkan_LIBRARIES} ${GLFW3_LIBRARY} imgui)
LinkTest(yqengine)


