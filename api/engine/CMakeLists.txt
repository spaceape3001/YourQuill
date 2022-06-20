if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(engine_test_source ./test.cpp)
endif()

set(yq_engine_sources
    ${engine_test_source}

    ./engine.cpp

    ./Application.hpp
    ./Application.ipp
    ./Asset.cpp
    ./Asset.hpp
    ./AssetCache.hpp
    ./AssetCompiler.hpp
    ./AssetLoader.hpp
    ./AssetInfoWriter.hpp
    ./Camera.cpp
    ./Camera.hpp
    ./CameraProxy.hpp
    ./CameraInfoWriter.hpp
    ./ImWidget.cpp
    ./ImWidget.hpp
    ./ImWidgetInfoWriter.hpp
    ./ImWindow.hpp
    ./ImWindow.cpp
    ./MainWindow.cpp
    ./MainWindow.hpp
    ./Pipeline.cpp
    ./PipelineBuilder.hpp
    ./PipelineConfig.hpp
    ./Rendered.cpp
    ./Rendered.hpp
    ./RenderedInfoWriter.hpp
    ./Render3D.hpp
    ./Render3D.cpp
    ./Render3DInfoWriter.hpp
    ./Shader.cpp
    ./Shader.hpp
    ./ShaderCache.hpp
    ./ShaderCompiler.hpp
    ./ShaderLoader.hpp
    ./ShaderInfoWriter.hpp
    ./Texture.cpp
    ./Texture.hpp
    ./TextureCache.hpp
    ./TextureLoader.hpp
    ./TextureInfoWriter.hpp
    
    ./enum/BorderColor.hpp   
    ./enum/ColorComponent.hpp    
    ./enum/ComponentSwizzle.hpp
    ./enum/CullMode.hpp   
    ./enum/DataFormat.hpp 
    ./enum/DescriptorType.hpp   
    ./enum/FilterType.hpp   
    ./enum/FrontFace.hpp    
    ./enum/ImageLayout.hpp
    ./enum/ImageTiling.hpp
    ./enum/ImageType.hpp
    ./enum/LogicOp.hpp    
    ./enum/MipmapMode.hpp    
    ./enum/PolygonMode.hpp
    ./enum/SamplerAddressMode.hpp
    ./enum/SamplerMipmapMode.hpp
    ./enum/StencilOp.hpp
    ./enum/SubpassContents.hpp
    ./enum/SystemAllocationScope.hpp
    ./enum/Topology.hpp
    
    ./vulqan/VqAllocator.hpp
    ./vulqan/VqAllocator.ipp
    ./vulqan/VqBuffer.hpp
    ./vulqan/VqBuffer.ipp
    ./vulqan/VqCommand.ipp
    ./vulqan/VqCommand.hpp
    ./vulqan/VqCommandBuffers.ipp
    ./vulqan/VqCommandBuffers.hpp
    ./vulqan/VqCommandPool.ipp
    ./vulqan/VqCommandPool.hpp
    ./vulqan/VqDescriptorPool.ipp
    ./vulqan/VqDescriptorPool.hpp
    ./vulqan/VqDevice.ipp
    ./vulqan/VqDevice.hpp
    ./vulqan/VqEnums.ipp
    ./vulqan/VqFence.ipp
    ./vulqan/VqFence.hpp
    ./vulqan/VqFencePool.ipp
    ./vulqan/VqFencePool.hpp
    ./vulqan/VqFrameBuffers.ipp
    ./vulqan/VqFrameBuffers.hpp
    ./vulqan/VqGPU.ipp
    ./vulqan/VqGPU.hpp
    ./vulqan/VqImageViews.ipp
    ./vulqan/VqImageViews.hpp
    ./vulqan/VqInternal.cpp
    ./vulqan/VqInternal.hpp
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
    ./vulqan/VqSwapchain.ipp
    ./vulqan/VqSwapchain.hpp
    ./vulqan/VqUtils.ipp
    ./vulqan/VqUtils.hpp
    ./vulqan/VqWindow.ipp
    ./vulqan/VqWindow.hpp
)

add_library(yqengine SHARED ${yq_engine_sources})
target_compile_definitions(yqengine PRIVATE VK_ENABLE_BETA_EXTENSIONS=1)
target_link_libraries(yqengine PUBLIC yqbasic yqconfig yqmath ${Vulkan_LIBRARIES} ${GLFW3_LIBRARY} imgui stb vma)
LinkTest(yqengine)


