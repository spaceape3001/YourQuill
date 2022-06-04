////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

/*
    YES, we're leveraging the "hello triangle" tutorial
    
    https://vulkan-tutorial.com/
*/

#include "VqApp.hpp"
#include "VqShaderStages.hpp"
#include "VqUtils.hpp"
#include "VqWindow.hpp"

#include <basic/CollectionUtils.hpp>
#include <basic/Logging.hpp>
#include <basic/meta/ObjectInfoWriter.hpp>
#include <engine/render/PipelineConfig.hpp>
#include <math/shape/Size2.hpp>
#include <math/vec/Vec2.hpp>

#include <algorithm>
#include <cassert>

#include <GLFW/glfw3.h>

YQ_OBJECT_IMPLEMENT(yq::engine::VqWindow)

namespace yq {
    namespace engine {
        void VqWindow::poll_events()
        {
            glfwPollEvents();
        }
        
        VqWindowInfo::VqWindowInfo(std::string_view kname, ObjectInfo& base, const std::source_location& sl) :
            ObjectInfo(kname, base, sl)
        {
        }

        ////////////////////////////////////////////////////////////////////////////////

        VqWindow::VqWindow(const WindowCreateInfo&i)
        {
            init(i);
        }
        
        VqWindow::~VqWindow()
        {
            kill();
        }

        bool VqWindow::init(const WindowCreateInfo& i)
        {
            VkInstance     inst  = VqApp::instance();
            if(!inst){
                yCritical() << "Vulkan has not been initialized!";
                return false;
            }
            
            const VqApp*   app    = VqApp::app();
            
                /*
                    First, determine our physical device (GPU, etc)
                */
            
            m_physical  = i.device;
            if(!m_physical){
                    //  NULL specification is fine, we grab the first/suitable one
                    //  TODO ... use the monitor hint
                m_physical  = vqFirstDevice();
                if(!m_physical){
                    yCritical() << "Cannot create window without any devices!";
                    return false;
                }
            }

                /*
                    --------------------------------------------------------------------------------------------------------
                    GLFW WINDOW STUFF
                    
                    NOTE, once this has been called, all future aborts will need to call
                    kill() to reset our state
                */

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_FLOATING, i.floating ? GLFW_TRUE : GLFW_FALSE);
            glfwWindowHint(GLFW_DECORATED, i.decorated ? GLFW_TRUE : GLFW_FALSE);
            glfwWindowHint(GLFW_RESIZABLE, i.resizable ? GLFW_TRUE : GLFW_FALSE);

            m_window = glfwCreateWindow(std::max(1,i.size.width()), std::max(1,i.size.height()), i.title, i.monitor.monitor(), nullptr);
            if(!m_window){
                yError() << "Unable to create window.";
                kill();
                return false;
            }
            
            glfwSetWindowUserPointer(m_window, this);
            
                //  NOTE ALL FAILURES AFTER THIS POINT REQUIRE A CALL TO KILL()!
            
                /*
                    --------------------------------------------------------------------------------------------------------
                    SURFACE
                */

            if(glfwCreateWindowSurface(inst, m_window, nullptr, &m_surface) != VK_SUCCESS){
                yCritical() << "Unable to create window surface!";
                kill();
                return false;
            }

                /*
                    --------------------------------------------------------------------------------------------------------
                    LOGICAL DEVICE
                */

            
            /*
                Create our logical device
            */
            
            std::vector<const char*> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };
        
            auto queueInfos         = vqFindQueueFamilies(m_physical, m_surface);
            if(!queueInfos.graphicsFamily.has_value()){
                yCritical() << "Unable to get a queue with graphic capability!";
                kill();
                return false;
            }
            if(!queueInfos.presentFamily.has_value()){
                yCritical() << "Unable to find a present queue!";
                kill();
                return false;
            }
        
            std::vector<VkDeviceQueueCreateInfo> qci;
            float quePri    = 1.0f;
            
            for(uint32_t i : std::set<uint32_t>({ queueInfos.graphicsFamily.value(), queueInfos.presentFamily.value()})){
                VqDeviceQueueCreateInfo info;
                info.queueFamilyIndex = i;
                info.queueCount = 1;
                info.pQueuePriorities    = &quePri;
                qci.push_back(info);
            }

            m_graphicsQueueFamily        = queueInfos.graphicsFamily.value();

        
            VkPhysicalDeviceFeatures    df{};
            VqDeviceCreateInfo          dci;
            dci.pQueueCreateInfos        = qci.data();
            dci.queueCreateInfoCount     = (uint32_t) qci.size();
            dci.pEnabledFeatures         = &df;
            
            dci.enabledLayerCount        = (uint32_t) app->m_layers.size();
            if(dci.enabledLayerCount)
                dci.ppEnabledLayerNames  = app->m_layers.data();
        
            dci.enabledExtensionCount    = (uint32_t) deviceExtensions.size();
            if(!deviceExtensions.empty())
                dci.ppEnabledExtensionNames = deviceExtensions.data();
            
            if(vkCreateDevice(m_physical, &dci, nullptr, &m_device) != VK_SUCCESS){
                yCritical() << "Unable to create logical device!";
                kill();
                return false;
            }
            
                /*
                    Graphics queues
                */
            vkGetDeviceQueue(m_device, queueInfos.graphicsFamily.value(), 0, &m_graphicsQueue);
            vkGetDeviceQueue(m_device, queueInfos.presentFamily.value(), 0, &m_presentQueue);
            
        
            auto    pmodes              = make_set(vqGetPhysicalDeviceSurfacePresentModesKHR(m_physical, m_surface));
            
            //for(auto& sf : vqGetPhysicalDeviceSurfaceFormatsKHR(m_physical, m_surface)){
                //yInfo() << "Format available... " << to_string(sf.format) << "/" << to_string(sf.colorSpace);
            //}

            //  cache details.....
        
            m_presentMode               = pmodes.contains(i.pmode) ? i.pmode : VK_PRESENT_MODE_FIFO_KHR;
                // I know this is available on my card, get smarter later.
            m_surfaceFormat.format      = VK_FORMAT_B8G8R8A8_SRGB;
            m_surfaceFormat.colorSpace  = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        
                /*
                    SWAP CHAIN
                */
                
            if(!m_swapChain.init(this)){
                kill();
                return false;
            }
            
            if(!m_renderPass.init(this)){
                kill();
                return false;
            }
                
            if(!m_frameBuffers.init(this)){
                kill();
                return false;
            }
            
            if(!m_command.init(this)){
                kill();
                return false;
            }
                
            if(!m_sync.init(this)){
                kill();
                return false;
            }
            
            set_clear(i.clear);
                
            return true;
        }


        void VqWindow::kill()
        {
            if(m_device){
                vkDeviceWaitIdle(m_device);
            }

            m_sync.kill(this);
            m_command.kill(this);
            m_frameBuffers.kill(this);
            m_renderPass.kill(this);
            m_swapChain.kill(this);
            
            if(m_device){
                vkDestroyDevice(m_device, nullptr);
                m_device   = nullptr;
            }

            if(m_surface){
                vkDestroySurfaceKHR(VqApp::instance(), m_surface, nullptr);
                m_surface  = nullptr;
            }

            if(m_window){
                glfwDestroyWindow(m_window);
                m_window    = nullptr;
            }
            
            m_physical = nullptr;
        }

        ////////////////////////////////////////////////////////////////////////////////

        bool VqWindow::Command::init(VqWindow* win)
        {
            VqQueueFamilyIndices queueFamilyIndices = vqFindQueueFamilies(win->m_physical, win->m_surface);

            VqCommandPoolCreateInfo poolInfo;
            poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
            if (vkCreateCommandPool(win->m_device, &poolInfo, nullptr, &pool) != VK_SUCCESS) {
                yError() << "Failed to create command pool!";
                return false;
            }

            VqCommandBufferAllocateInfo allocInfo;
            allocInfo.commandPool = pool;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandBufferCount = 1;

            if (vkAllocateCommandBuffers(win->m_device, &allocInfo, &buffer) != VK_SUCCESS) {
                yError () << "Failed to allocate command buffers!";
                return false;
            }
            return true;
        }
        
        void VqWindow::Command::kill(VqWindow* win)
        {
            buffer  = nullptr;
            if(pool){
                vkDestroyCommandPool(win->m_device, pool, nullptr);
                pool    = nullptr;
            }
        }


        ////////////////////////////////////////////////////////////////////////////////

        bool VqWindow::FrameBuffers::init(VqWindow* win)
        {
            buffers.resize(win -> m_swapChain.imageViews.size(), nullptr);
            for (size_t i = 0; i < win -> m_swapChain.imageViews.size(); i++) {
                VkImageView attachments[] = {
                    win -> m_swapChain.imageViews[i]
                };

                VqFramebufferCreateInfo framebufferInfo;
                framebufferInfo.renderPass = win->m_renderPass.handle;
                framebufferInfo.attachmentCount = 1;
                framebufferInfo.pAttachments = attachments;
                framebufferInfo.width = win->swap_width();
                framebufferInfo.height = win->swap_height();
                framebufferInfo.layers = 1;

                if (vkCreateFramebuffer(win->m_device, &framebufferInfo, nullptr, &buffers[i]) != VK_SUCCESS) {
                    yCritical() << "Failed to create framebuffer!";
                    return false;
                }
            }
            return true;
        }
        
        void VqWindow::FrameBuffers::kill(VqWindow* win)
        {
            for (VkFramebuffer fb : buffers) {
                if(fb)
                    vkDestroyFramebuffer(win->m_device, fb, nullptr);
            }
        }

        ////////////////////////////////////////////////////////////////////////////////

        bool    VqWindow::Pipeline::init(VqWindow*win, const PipelineConfig&cfg)
        {
            VqShaderStages stages(*win, cfg.shaders);

            VqPipelineVertexInputStateCreateInfo    vertexInfo;
            vertexInfo.vertexBindingDescriptionCount    = 0;
            vertexInfo.pVertexBindingDescriptions       = nullptr;
            vertexInfo.vertexAttributeDescriptionCount  = 0;
            vertexInfo.pVertexAttributeDescriptions     = nullptr;
            
            VqPipelineInputAssemblyStateCreateInfo  inputAssembly;
            inputAssembly.topology                  = (VkPrimitiveTopology) cfg.topology.value();
            inputAssembly.primitiveRestartEnable    = VK_FALSE;
            
            VkViewport viewport = win -> swap_def_viewport();

            VkRect2D scissor = win -> swap_def_scissor();
            
            VqPipelineViewportStateCreateInfo   viewportState{};
            viewportState.viewportCount = 1;
            viewportState.pViewports = &viewport;
            viewportState.scissorCount = 1;
            viewportState.pScissors = &scissor;
            
            VqPipelineRasterizationStateCreateInfo  rasterizer;
            rasterizer.depthClampEnable = VK_FALSE;
            rasterizer.rasterizerDiscardEnable = VK_FALSE;
            rasterizer.polygonMode = (VkPolygonMode) cfg.polymode.value();
            rasterizer.lineWidth = 1.0f;
            rasterizer.cullMode = (VkCullModeFlags) cfg.cull.value();
            rasterizer.frontFace = (VkFrontFace) cfg.front.value();
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

            if (vkCreatePipelineLayout(win->m_device, &pipelineLayoutInfo, nullptr, &layout) != VK_SUCCESS) {
                yError() << "Failed to create pipeline layout!";
                return false;
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
            pipelineInfo.layout = layout;
            pipelineInfo.renderPass = win -> m_renderPass.handle;
            pipelineInfo.subpass = 0;             
            pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
            pipelineInfo.basePipelineIndex = -1; // Optional        

            if (vkCreateGraphicsPipelines(win -> m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
                yError() << "Failed to create graphics pipeline!";
                return false;
            }
            return true;
        }
        
        void    VqWindow::Pipeline::kill(VqWindow*win)
        {
            if(pipeline){
                vkDestroyPipeline(win->m_device, pipeline, nullptr);
                pipeline    = nullptr;
            }
            if(layout){
                vkDestroyPipelineLayout(win->m_device, layout, nullptr);
                layout      = nullptr;
            }
                
        }
        
        ////////////////////////////////////////////////////////////////////////////////

        bool VqWindow::RenderPass::init(VqWindow* win)
        {
                //  Render pass
            VkAttachmentDescription colorAttachment{};
            colorAttachment.format = win->m_surfaceFormat.format;
            colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
            colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;        
            colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            
            VkAttachmentReference colorAttachmentRef{};
            colorAttachmentRef.attachment = 0;
            colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkSubpassDescription subpass{};
            subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;        

            subpass.colorAttachmentCount = 1;
            subpass.pColorAttachments = &colorAttachmentRef;
            
            VkRenderPassCreateInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            renderPassInfo.attachmentCount = 1;
            renderPassInfo.pAttachments = &colorAttachment;
            renderPassInfo.subpassCount = 1;
            renderPassInfo.pSubpasses = &subpass;

            if (vkCreateRenderPass(win->m_device, &renderPassInfo, nullptr, &handle) != VK_SUCCESS) {
                yCritical() << "Unable to create the render pass!";
                return false;
            } else
                return true;
        }
        
        void VqWindow::RenderPass::kill(VqWindow* win)
        {
            if(handle){
                vkDestroyRenderPass(win->m_device, handle, nullptr);
                handle    = nullptr;
            }
        }

        ////////////////////////////////////////////////////////////////////////////////

        VkRect2D    VqWindow::SwapChain::def_scissor() const
        {
            VkRect2D    ret{};
            ret.offset  = { 0, 0 };
            ret.extent  = extents;
            return ret;
        }
        
        VkViewport  VqWindow::SwapChain::def_viewport() const
        {
            VkViewport  ret{};
            ret.x = 0.0f;
            ret.y = 0.0f;
            ret.width = (float) extents.width;
            ret.height = (float) extents.height;
            ret.minDepth = 0.0f;
            ret.maxDepth = 1.0f;
            return ret;
        }
            
        bool    VqWindow::SwapChain::init(VqWindow* win)
        {
            VkSurfaceCapabilitiesKHR    capabilities;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(win->m_physical, win->m_surface, &capabilities);
            if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                extents = capabilities.currentExtent;
            } else {
                int w, h;
                glfwGetFramebufferSize(win->m_window, &w, &h);
                extents = {};
                extents.width    = std::clamp((uint32_t) w, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
                extents.height   = std::clamp((uint32_t) h, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
            }

            
            minImageCount               = capabilities.minImageCount;
            if(minImageCount < 2)
                minImageCount   = 2;
            imageCount                  = minImageCount + 1;
            if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
                imageCount = capabilities.maxImageCount;
            }

            VqSwapchainCreateInfoKHR    createInfo;
            createInfo.surface          = win->m_surface;
            createInfo.minImageCount    = imageCount;
            createInfo.imageFormat      = win->m_surfaceFormat.format;
            createInfo.imageColorSpace  = win->m_surfaceFormat.colorSpace;
            createInfo.imageExtent      = extents;
            createInfo.imageArrayLayers = 1;    // we're not steroscopic
            createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            
            auto indices                = vqFindQueueFamilies(win->m_physical, win->m_surface);
            uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};
            if (indices.graphicsFamily != indices.presentFamily) {
                createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                createInfo.queueFamilyIndexCount = 2;
                createInfo.pQueueFamilyIndices = queueFamilyIndices;
            } else {
                createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                createInfo.queueFamilyIndexCount = 0; // Optional
                createInfo.pQueueFamilyIndices = nullptr; // Optional
            }        
            createInfo.preTransform     = capabilities.currentTransform;
            createInfo.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createInfo.presentMode      = win->m_presentMode;
            createInfo.clipped          = VK_TRUE;
            
                // TEMPORARY UNTIL WE GET THE NEW ONE
            createInfo.oldSwapchain = VK_NULL_HANDLE;
            
            if (vkCreateSwapchainKHR(win->m_device, &createInfo, nullptr, &handle) != VK_SUCCESS){
                handle   = nullptr;
                yCritical() << "Failed to create the SWAP chain!";
                return false;
            }

            vkGetSwapchainImagesKHR(win->m_device, handle, &imageCount, nullptr);
            images.resize(imageCount);
            vkGetSwapchainImagesKHR(win->m_device, handle, &imageCount, images.data());    

            imageViews.resize(images.size(), nullptr);
            for(size_t i=0;i<images.size(); ++i){
                VqImageViewCreateInfo   createInfo;
                createInfo.image        = images[i];
                createInfo.viewType     = VK_IMAGE_VIEW_TYPE_2D;
                createInfo.format       = win->m_surfaceFormat.format;
                createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                createInfo.subresourceRange.baseMipLevel = 0;
                createInfo.subresourceRange.levelCount = 1;
                createInfo.subresourceRange.baseArrayLayer = 0;
                createInfo.subresourceRange.layerCount = 1;
                if(vkCreateImageView(win->m_device, &createInfo, nullptr, &imageViews[i]) != VK_SUCCESS) {
                    yCritical() << "Failed to create one of the Swap Image Viewers!";
                }
            }
            
            return true;
        }
        
        void    VqWindow::SwapChain::kill(VqWindow* win)
        {
            for(auto iv : imageViews){
                if(iv)
                    vkDestroyImageView(win->m_device, iv, nullptr);
            }
            imageViews.clear();
            if(handle){
                vkDestroySwapchainKHR(win->m_device, handle, nullptr);
                handle  = nullptr;
            }
        }

        ////////////////////////////////////////////////////////////////////////////////

        bool VqWindow::Sync::init(VqWindow* win)
        {
            VqSemaphoreCreateInfo   semaphoreInfo;
            
            if (vkCreateSemaphore(win->m_device, &semaphoreInfo, nullptr, &imageAvailable) != VK_SUCCESS ||
                vkCreateSemaphore(win->m_device, &semaphoreInfo, nullptr, &renderFinished) != VK_SUCCESS) {
                
                yError() << "Failed to create semaphores!";
                return false;
            }
            
            inFlightFence   = VqFence(*win);
            if(!inFlightFence.good()){
                yError() << "Failed to create semaphores!";
                return false;
            }
            return true;
        }
        
        void VqWindow::Sync::kill(VqWindow* win)
        {
            if(imageAvailable){
                vkDestroySemaphore(win->m_device, imageAvailable, nullptr);
                imageAvailable  = nullptr;
            }
            if(renderFinished){
                vkDestroySemaphore(win->m_device, renderFinished, nullptr);
                renderFinished  = nullptr;
            }
            inFlightFence   = {};
        }
        
        ////////////////////////////////////////////////////////////////////////////////

        bool    VqWindow::record(VkCommandBuffer commandBuffer, uint32_t imageIndex)
        {
            VqCommandBufferBeginInfo beginInfo;
            beginInfo.flags = 0; // Optional
            beginInfo.pInheritanceInfo = nullptr; // Optional

            if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
                yError() << "Failed to begin recording command buffer!";
                return false;
            }

            VqRenderPassBeginInfo renderPassInfo;
            renderPassInfo.renderPass = m_renderPass.handle;
            renderPassInfo.framebuffer = m_frameBuffers.buffers[imageIndex];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = m_swapChain.extents;

            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &m_clear;
            vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        
            draw_vulqan(commandBuffer);

            vkCmdEndRenderPass(commandBuffer);

            if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
                yError() << "Failed to record command buffer!";
                return false;
            }
            return true;
        }
        
        bool    VqWindow::draw()
        {
            m_sync.inFlightFence.wait_reset();
            uint32_t imageIndex = 0;
            vkAcquireNextImageKHR(m_device, m_swapChain.handle, UINT64_MAX, m_sync.imageAvailable, VK_NULL_HANDLE, &imageIndex);
            vkResetCommandBuffer(m_command.buffer, 0);
            
            if(!record(m_command.buffer, imageIndex))
                return false;
            
            VqSubmitInfo submitInfo;

            VkSemaphore waitSemaphores[] = {m_sync.imageAvailable};
            VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = waitSemaphores;
            submitInfo.pWaitDstStageMask = waitStages;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &m_command.buffer;

            VkSemaphore signalSemaphores[] = {m_sync.renderFinished};
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = signalSemaphores;

            if (vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, m_sync.inFlightFence) != VK_SUCCESS) {
                yError() << "Failed to submit draw command buffer!";
                return false;
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
            VkSwapchainKHR swapChains[] = {m_swapChain.handle};
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = swapChains;
            presentInfo.pImageIndices = &imageIndex;
            presentInfo.pResults = nullptr; // Optional
            vkQueuePresentKHR(m_presentQueue, &presentInfo);
            return true;
        }
        
        
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////

        void VqWindow::attention()
        {
            if(m_window)
                glfwRequestWindowAttention(m_window);
        }

        void VqWindow::close()
        {
            if(m_window)
                glfwSetWindowShouldClose(m_window, GLFW_TRUE);
        }

        VkCommandBuffer     VqWindow::command_buffer() const
        {
            return m_command.buffer;
        }

        VkCommandPool       VqWindow::command_pool() const
        {
            return m_command.pool;
        }

        void VqWindow::focus()
        {
            if(m_window)
                glfwFocusWindow(m_window);
        }

        int  VqWindow::height() const
        {
            if(!m_window)
                return 0;
        
            int ret;
            glfwGetWindowSize(m_window, nullptr, &ret);
            return ret;
        }

        void VqWindow::hide()
        {
            if(m_window)
                glfwHideWindow(m_window);
        }

        void VqWindow::iconify()
        {
            if(m_window)
                glfwIconifyWindow(m_window);
        }

        bool        VqWindow::is_decorated() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_DECORATED) != 0;
        }
        
        bool        VqWindow::is_focused() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_FOCUSED ) != 0;
        }
        
        bool        VqWindow::is_floating() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_FLOATING) != 0;
        }
        
        bool        VqWindow::is_fullscreen() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowMonitor(m_window) != nullptr;
        }
        
        bool        VqWindow::is_hovered() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_HOVERED) != 0;
        }
        
        bool        VqWindow::is_iconified() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0;
        }

        bool        VqWindow::is_maximized() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_MAXIMIZED) != 0;
        }
        
        bool        VqWindow::is_resizable() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_RESIZABLE) != 0;
        }
        
        bool        VqWindow::is_visible() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_VISIBLE) != 0;
        }
        

        void        VqWindow::maximize()
        {
            if(m_window)
                glfwMaximizeWindow(m_window);
        }

        VqMonitor   VqWindow::monitor() const
        {
            if(m_window)
                return VqMonitor(glfwGetWindowMonitor(m_window));
            return VqMonitor();
        }

        Vec2I       VqWindow::position() const
        {
            if(!m_window)
                return {};
            Vec2I   ret;
            glfwGetWindowPos(m_window, &ret.x, &ret.y);
            return ret;
        }


        VkRenderPass VqWindow::render_pass() const
        {
            return m_renderPass.handle;
        }

        void        VqWindow::restore()
        {
            if(m_window)
                glfwRestoreWindow(m_window);
        }

        void        VqWindow::set_clear(const ColorRgbF&i)
        {
            m_clear = VkClearValue{{{ i.red, i.green, i.blue, i.alpha }}};
        }

        void        VqWindow::set_position(const Vec2I& pos)
        {
            set_position(pos.x, pos.y);
        }
        
        void        VqWindow::set_position(int x, int y)
        {
            if(m_window){
                glfwSetWindowPos(m_window, x, y);
            }
        }

        void        VqWindow::set_size(const Size2I& sz)
        {
            set_size(sz.x, sz.y);
        }

        void        VqWindow::set_size(int w, int h)
        {
            if(m_window){
                glfwSetWindowSize(m_window, std::max(1, w), std::max(1, h));
            }
        }

        void        VqWindow::set_title(const char*z)
        {
            if(m_window && z){
                glfwSetWindowTitle(m_window, z);
            }
        }

        void        VqWindow::set_title(const std::string&z)
        {
            set_title(z.c_str());
        }

        bool        VqWindow::should_close() const
        {
            if(!m_window) 
                return true;
            return glfwWindowShouldClose(m_window);
        }

        void        VqWindow::show()
        {
            if(m_window)
                glfwShowWindow(m_window);
        }

        Size2I      VqWindow::size() const
        {
            if(!m_window)
                return {};
            Size2I  ret;
            glfwGetWindowSize(m_window, &ret.x, &ret.y);
            return ret;
        }

        VkRect2D    VqWindow::swap_def_scissor() const
        {
            return m_swapChain.def_scissor();
        }
        
        VkViewport  VqWindow::swap_def_viewport() const
        {
            return m_swapChain.def_viewport();
        }

        uint32_t    VqWindow::swap_image_count() const
        {
            return m_swapChain.imageCount;
        }

        uint32_t    VqWindow::swap_height() const
        {
            return m_swapChain.extents.height;
        }

        uint32_t    VqWindow::swap_min_image_count() const
        {
            return m_swapChain.minImageCount;
        }

        uint32_t    VqWindow::swap_width() const
        {
            return m_swapChain.extents.width;
        }
        
        int  VqWindow::width() const
        {
            if(!m_window)
                return 0;
        
            int ret;
            glfwGetWindowSize(m_window, &ret, nullptr);
            return ret;
        }
        
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        
        YQ_INVOKE(
            writer<VqWindow>();
        )
    }

}
