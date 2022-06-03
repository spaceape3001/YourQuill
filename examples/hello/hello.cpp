////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

/*

    This example is about demonstrating the use of the API to display a 
    simple triangle (in 2D mode) statically on the window.
    
    Note, it's heavily leveraged/inspired/taken from the Hello Triangle SDK example 
    located at https://vulkan-tutorial.com/Introduction
*/

//  Also it's about me developing the API in the first place .... :)


#include <basic/Logging.hpp>
#include <basic/PluginLoader.hpp>
#include <basic/Safety.hpp>
#include <basic/meta/Meta.hpp>
#include <engine/shader/Shader.hpp>
#include <engine/vulqan/VqApp.hpp>
#include <engine/vulqan/VqUtils.hpp>
#include <engine/vulqan/VqFence.hpp>
#include <engine/vulqan/VqFencePool.hpp>
#include <engine/vulqan/VqPipeline.hpp>
#include <engine/vulqan/VqShaderStages.hpp>
#include <engine/vulqan/VqWindow.hpp>
#include <iostream>

using namespace yq;

struct HelloWin : public VqWindow {
    YQ_OBJECT_DECLARE(HelloWin, VqWindow)
    
    HelloWin(const WindowCreateInfo& wci) : VqWindow(wci)
    {
    }
};

YQ_OBJECT_IMPLEMENT(HelloWin)


struct HelloApp {
    
    Ref<HelloWin>       window;
    ShaderPtr           vert, frag;
    VkPipelineLayout    pipelineLayout  = nullptr;
    VkPipeline          graphicsPipeline = nullptr;
    VkCommandPool       commandPool   = nullptr;
    VkCommandBuffer     commandBuffer = nullptr;
    VkSemaphore         imageAvailableSemaphore;
    VkSemaphore         renderFinishedSemaphore;
    VqFence             inFlightFence;
    
    HelloApp()
    {
            
        WindowCreateInfo      wi;
        wi.title        = "Hello WORLD!";
        wi.resizable    = false;
        wi.size.x       = 1920;
        window          = new HelloWin(wi);
        vert            = Shader::load("examples/hello/hello.vert");
        frag            = Shader::load("examples/hello/hello.frag");
        
        if(!vert)
            throw std::runtime_error("No vertex shader");
        if(!frag)
            throw std::runtime_error("No fragment shader");
            
        createGraphicsPipeline();
        createCommandPool();
        createCommandBuffer();
        createSyncObjects();
    }
    
    ~HelloApp()
    {
        vkDestroySemaphore(window->m_device, imageAvailableSemaphore, nullptr);
        vkDestroySemaphore(window->m_device, renderFinishedSemaphore, nullptr);
        inFlightFence   = {};
        vkDestroyCommandPool(window->m_device, commandPool, nullptr);

        vkDestroyPipeline(window->m_device, graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(window->m_device, pipelineLayout, nullptr);
    }
    
    void    createGraphicsPipeline()
    {
        VqShaderStages stages(*window, {vert, frag});

        VqPipelineVertexInputStateCreateInfo    vertexInfo;
        vertexInfo.vertexBindingDescriptionCount    = 0;
        vertexInfo.pVertexBindingDescriptions       = nullptr;
        vertexInfo.vertexAttributeDescriptionCount  = 0;
        vertexInfo.pVertexAttributeDescriptions     = nullptr;
        
        VqPipelineInputAssemblyStateCreateInfo  inputAssembly;
        inputAssembly.topology                  = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable    = VK_FALSE;
        
        VkViewport viewport = window -> swap_def_viewport();

        VkRect2D scissor = window -> swap_def_scissor();
        
        VqPipelineViewportStateCreateInfo   viewportState{};
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;
        
        VqPipelineRasterizationStateCreateInfo  rasterizer;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f; // Optional
        rasterizer.depthBiasClamp = 0.0f; // Optional
        rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

        VqPipelineMultisampleStateCreateInfo multisampling;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f; // Optional
        multisampling.pSampleMask = nullptr; // Optional
        multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
        multisampling.alphaToOneEnable = VK_FALSE; // Optional

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
        
        VqPipelineColorBlendStateCreateInfo colorBlending;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f; // Optional
        colorBlending.blendConstants[1] = 0.0f; // Optional
        colorBlending.blendConstants[2] = 0.0f; // Optional
        colorBlending.blendConstants[3] = 0.0f; // Optional

        VqPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.setLayoutCount = 0; // Optional
        pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

        if (vkCreatePipelineLayout(window->m_device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }

        VqGraphicsPipelineCreateInfo pipelineInfo;
        pipelineInfo << stages;
        
        pipelineInfo.pVertexInputState = &vertexInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = nullptr; // Optional
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = nullptr; // Optional   
        pipelineInfo.layout = pipelineLayout;
        pipelineInfo.renderPass = window -> m_renderPass.handle;
        pipelineInfo.subpass = 0;             
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex = -1; // Optional        

        if (vkCreateGraphicsPipelines(window -> m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }

    }
    
    void createCommandPool() 
    {
        VqQueueFamilyIndices queueFamilyIndices = vqFindQueueFamilies(window->m_physical, window->m_surface);

        VqCommandPoolCreateInfo poolInfo;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
        if (vkCreateCommandPool(window->m_device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool!");
        }
    }

    void createCommandBuffer() 
    {
        VqCommandBufferAllocateInfo allocInfo;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(window->m_device, &allocInfo, &commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void createSyncObjects() 
    {
        VqSemaphoreCreateInfo   semaphoreInfo;
        
        if (vkCreateSemaphore(window->m_device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
            vkCreateSemaphore(window->m_device, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS) {
            throw std::runtime_error("failed to create semaphores!");
        }
        
        inFlightFence   = VqFence(*window);
        if(!inFlightFence.good())
            throw std::runtime_error("failed to create semaphores!");
    }
    
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) 
    {
        VqCommandBufferBeginInfo beginInfo;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VqRenderPassBeginInfo renderPassInfo;
        renderPassInfo.renderPass = window->m_renderPass.handle;
        renderPassInfo.framebuffer = window->m_frameBuffers.buffers[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = window->m_swapChain.extents;

        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;
        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
        vkCmdDraw(commandBuffer, 3, 1, 0, 0);
        vkCmdEndRenderPass(commandBuffer);
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
    
    void    drawFrame()
    {
        inFlightFence.wait_reset();
        uint32_t imageIndex = 0;
        vkAcquireNextImageKHR(window->m_device, window->m_swapChain.handle, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
        vkResetCommandBuffer(commandBuffer, 0);
        recordCommandBuffer(commandBuffer, imageIndex);
        
        VqSubmitInfo submitInfo;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(window->m_graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }
            
        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        
        VqRenderPassCreateInfo renderPassInfo;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        VqPresentInfoKHR presentInfo;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        VkSwapchainKHR swapChains[] = {window->m_swapChain.handle};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional
        vkQueuePresentKHR(window->m_presentQueue, &presentInfo);
    }

    void    run()
    {
        while(!window->should_close()){
            VqWindow::poll_events();
            drawFrame();
        }
        
        vkDeviceWaitIdle(window->m_device);
    }
};




int main(int argc, char* argv[])
{
    AppCreateInfo        vi;
    VqApp app(argc, argv, vi);
    load_plugin_dir("plugin");
    app.finalize();
    
    HelloApp        app2;
    app2.run();
    
    std::cout << "Hello World!\n";
    return 0;
}
