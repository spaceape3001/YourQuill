if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(engine_test_source ./test.cpp)
endif()

set(yq_engine_sources
    ${engine_test_source}

    ./Application.cpp
    ./Application.cpp
    ./Window.cpp
    ./Window.hpp
    
    ./asset/Asset.cpp
    ./asset/Asset.hpp
    ./asset/AssetCache.hpp
    ./asset/AssetCompiler.hpp
    ./asset/AssetLoader.hpp
    ./asset/AssetInfoWriter.hpp
    
    ./enum/engine_enums.cpp
    ./enum/BorderColor.hpp   
    ./enum/ColorComponent.hpp    
    ./enum/CullMode.hpp   
    ./enum/DataFormat.hpp 
    ./enum/DescriptorType.hpp   
    ./enum/FilterType.hpp   
    ./enum/FrontFace.hpp    
    ./enum/LogicOp.hpp    
    ./enum/MipmapMode.hpp    
    ./enum/PolygonMode.hpp
    ./enum/StencilOp.hpp
    ./enum/Topology.hpp
    
    ./pipeline/Pipeline.cpp
    ./pipeline/PipelineBuilder.hpp
    ./pipeline/PipelineConfig.hpp

    ./render/Renderable.cpp
    ./render/Renderable.hpp
    
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
    
    ./vulqan/vulqan.cpp
    ./vulqan/VqEnums.cpp
    ./vulqan/VqFence.cpp
    ./vulqan/VqFence.hpp
    ./vulqan/VqFencePool.cpp
    ./vulqan/VqFencePool.hpp
    ./vulqan/VqLogging.cpp
    ./vulqan/VqLogging.hpp
    ./vulqan/VqMonitor.cpp
    ./vulqan/VqMonitor.hpp
    ./vulqan/VqShaderStages.cpp
    ./vulqan/VqShaderStages.hpp
    ./vulqan/VqStructs.hpp
    ./vulqan/VqSurface.ipp
    ./vulqan/VqSurface.hpp
    ./vulqan/VqUtils.cpp
    ./vulqan/VqUtils.hpp
    ./vulqan/VqWindow.ipp
    ./vulqan/VqWindow.hpp
)

add_library(yqengine SHARED ${yq_engine_sources})
target_link_libraries(yqengine PUBLIC yqbasic yqconfig yqmath ${Vulkan_LIBRARIES} ${GLFW3_LIBRARY} imgui stb)
LinkTest(yqengine)


