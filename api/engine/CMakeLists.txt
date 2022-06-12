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
    ./vulqan/VqCommandBuffers.ipp
    ./vulqan/VqCommandBuffers.hpp
    ./vulqan/VqCommandPool.ipp
    ./vulqan/VqCommandPool.hpp
    ./vulqan/VqDescriptorPool.ipp
    ./vulqan/VqDescriptorPool.hpp
    ./vulqan/VqEnums.ipp
    ./vulqan/VqFence.ipp
    ./vulqan/VqFence.hpp
    ./vulqan/VqFencePool.ipp
    ./vulqan/VqFencePool.hpp
    ./vulqan/VqGPU.ipp
    ./vulqan/VqGPU.hpp
    ./vulqan/VqLogging.ipp
    ./vulqan/VqLogging.hpp
    ./vulqan/VqMonitor.ipp
    ./vulqan/VqMonitor.hpp
    ./vulqan/VqPipeline.cpp
    ./vulqan/VqPipeline.hpp
    ./vulqan/VqQueues.ipp
    ./vulqan/VqQueues.hpp
    ./vulqan/VqRenderPass.ipp
    ./vulqan/VqRenderPass.hpp
    ./vulqan/VqSemaphore.ipp
    ./vulqan/VqSemaphore.hpp
    ./vulqan/VqShaderStages.ipp
    ./vulqan/VqShaderStages.hpp
    ./vulqan/VqStructs.hpp
    ./vulqan/VqSurface.ipp
    ./vulqan/VqSurface.hpp
    ./vulqan/VqUtils.ipp
    ./vulqan/VqUtils.hpp
    ./vulqan/VqWindow.ipp
    ./vulqan/VqWindow.hpp
)

add_library(yqengine SHARED ${yq_engine_sources})
target_compile_definitions(yqengine PRIVATE VK_ENABLE_BETA_EXTENSIONS=1)
target_link_libraries(yqengine PUBLIC yqbasic yqconfig yqmath ${Vulkan_LIBRARIES} ${GLFW3_LIBRARY} imgui stb vma)
LinkTest(yqengine)


