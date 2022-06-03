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
#include <engine/render/PipelineConfig.hpp>
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
    
    Pipeline                triangle;
    
    HelloWin(const WindowCreateInfo& wci) : VqWindow(wci)
    {
        ShaderPtr   vert = Shader::load("examples/hello/hello.vert");
        ShaderPtr   frag = Shader::load("examples/hello/hello.frag");
        if(!vert)
            throw std::runtime_error("No vertex shader");
        if(!frag)
            throw std::runtime_error("No fragment shader");
            

        PipelineConfig      cfg;
        cfg.shaders     = { vert, frag };
        if(!triangle.init(this, cfg))
            throw std::runtime_error("Unable to create pipeline!");
    }
    
    ~HelloWin()
    {
        triangle.kill(this);
    }
    
    void        draw(VkCommandBuffer cmdbuf)
    {
        vkCmdBindPipeline(cmdbuf, VK_PIPELINE_BIND_POINT_GRAPHICS, triangle.pipeline);
        vkCmdDraw(cmdbuf, 3, 1, 0, 0);
    }
};

YQ_OBJECT_IMPLEMENT(HelloWin)


struct HelloApp {
    
    Ref<HelloWin>       window;
    ShaderPtr           vert, frag;
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
    
        window -> draw(commandBuffer);

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
