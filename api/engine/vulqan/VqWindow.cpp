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
#include <basic/Safety.hpp>
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

        bool VqWindow::init_physical(const WindowCreateInfo& i)
        {
            if(m_physical)
                return true;
        
            m_physical  = i.device;
            if(!m_physical){
                m_physical  = vqFirstDevice();
                if(!m_physical){
                    yCritical() << "Cannot create window without any devices!";
                    return false;
                }
            }
            return true;
        }

        bool VqWindow::init_window(const WindowCreateInfo& i)
        {
            if(m_window)    // already initialized
                return true;
                
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
            return true;
        }

        bool VqWindow::init_surface()
        {
            if(m_surface)           // already initialized!
                return true;
            if(glfwCreateWindowSurface(VqApp::instance(), m_window, nullptr, &m_surface) != VK_SUCCESS){
                yCritical() << "Unable to create window surface!";
                kill();
                return false;
            }
            return true;
        }

        bool VqWindow::init_logical()
        {
            if(m_device)
                return true;    // already initialized!
                
            std::vector<const char*> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };

            auto queueInfos         = vqFindQueueFamilies(m_physical, m_surface);
            if(!queueInfos.graphicsFamily.has_value()){
                yCritical() << "Unable to get a queue with graphic capability!";
                return false;
            }
            if(!queueInfos.presentFamily.has_value()){
                yCritical() << "Unable to find a present queue!";
                return false;
            }
        
            std::vector<VkDeviceQueueCreateInfo> qci;
            float quePri    = 1.0f;

            m_graphics.family           =   queueInfos.graphicsFamily.value();
            m_present.family            =   queueInfos.presentFamily.value();
            
            for(uint32_t i : std::set<uint32_t>({ m_graphics.family, m_present.family })){
                VqDeviceQueueCreateInfo info;
                info.queueFamilyIndex = i;
                info.queueCount = 1;
                info.pQueuePriorities    = &quePri;
                qci.push_back(info);
            }

            const VqApp*   app    = VqApp::app();
            if(!app){
                yCritical() << "Unintialized or no application present!";
                return false;
            }
        
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
                return false;
            }
            
                /*
                    Graphics queues
                */
            vkGetDeviceQueue(m_device, m_graphics.family, 0, &m_graphics.queue);
            vkGetDeviceQueue(m_device, m_present.family, 0, &m_present.queue);
            return true;
        }
        
        bool    VqWindow::init_command_pool()
        {
            if(m_commandPool)   // already initialized
                return true;
                
            VqCommandPoolCreateInfo poolInfo;
            poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            poolInfo.queueFamilyIndex = m_graphics.family;
            if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
                yError() << "Failed to create command pool!";
                return false;
            }
            return true;
        }

        bool VqWindow::init_render_pass()
        {
            if(m_renderPass)
                return true;
                
                //  Render pass
            VkAttachmentDescription colorAttachment{};
            colorAttachment.format = m_surfaceFormat;
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

            if (vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS) {
                yCritical() << "Unable to create the render pass!";
                return false;
            } 

            return true;
        }
        
        bool VqWindow::init_sync()
        {
            bool    success = true;
            if(!m_imageAvailableSemaphore){
                VqSemaphoreCreateInfo   semaphoreInfo;
                if(vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS){
                    yCritical() << "Unable to create semaphore for available images!";
                    success = false;
                }
            }
            if(!m_renderFinishedSemaphore){
                VqSemaphoreCreateInfo   semaphoreInfo;
                if(vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphore) != VK_SUCCESS){
                    yCritical() << "Unable to create semaphore for finished rendering!";
                    success = false;
                }
            }
            
            if(!m_inFlightFence){
                m_inFlightFence   = VqFence(*this);
                if(!m_inFlightFence.good()){
                    yError() << "Failed to create fence!";
                    success = false;
                }
            }
            
            return success;
            
        }

        bool VqWindow::init(DynamicStuff&ds, VkSwapchainKHR old)
        {
            //  ----------------------------
            //      SWAP CHAIN       
            //  ----------------------------
            
            VkSurfaceCapabilitiesKHR    capabilities;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physical, m_surface, &capabilities);
            if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                ds.extents = capabilities.currentExtent;
            } else {
                int w, h;
                glfwGetFramebufferSize(m_window, &w, &h);
                ds.extents = {};
                ds.extents.width    = std::clamp((uint32_t) w, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
                ds.extents.height   = std::clamp((uint32_t) h, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
            }

            ds.minImageCount               = capabilities.minImageCount;
            if(ds.minImageCount < 2)
                ds.minImageCount   = 2;
            ds.imageCount                  = ds.minImageCount + 1;
            if (capabilities.maxImageCount > 0 && ds.imageCount > capabilities.maxImageCount) {
                ds.imageCount = capabilities.maxImageCount;
            }

            VqSwapchainCreateInfoKHR    createInfo;
            createInfo.surface          = m_surface;
            createInfo.minImageCount    = ds.imageCount;
            createInfo.imageFormat      = m_surfaceFormat;
            createInfo.imageColorSpace  = m_surfaceColorSpace;
            createInfo.imageExtent      = ds.extents;
            createInfo.imageArrayLayers = 1;    // we're not steroscopic
            createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            
            uint32_t queueFamilyIndices[] = {m_graphics.family, m_present.family};
            if (m_graphics.family != m_present.family) {
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
            createInfo.presentMode      = m_present.mode;
            createInfo.clipped          = VK_TRUE;
            
                // TEMPORARY UNTIL WE GET THE NEW ONE
            createInfo.oldSwapchain = old;
            
            if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &ds.swapChain) != VK_SUCCESS){
                ds.swapChain   = nullptr;
                yCritical() << "Failed to create the SWAP chain!";
                return false;
            }

            //  ----------------------------
            //      IMAGES
            //  ----------------------------

            vkGetSwapchainImagesKHR(m_device, ds.swapChain, &ds.imageCount, nullptr);
            ds.images.resize(ds.imageCount, nullptr);
            vkGetSwapchainImagesKHR(m_device, ds.swapChain, &ds.imageCount, ds.images.data());    

            //  ----------------------------
            //      IMAGE VIEWS       
            //  ----------------------------

            ds.imageViews.resize(ds.images.size(), nullptr);
            for(size_t i=0;i<ds.images.size(); ++i){
                VqImageViewCreateInfo   createInfo;
                createInfo.image        = ds.images[i];
                createInfo.viewType     = VK_IMAGE_VIEW_TYPE_2D;
                createInfo.format       = m_surfaceFormat;
                createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                createInfo.subresourceRange.baseMipLevel = 0;
                createInfo.subresourceRange.levelCount = 1;
                createInfo.subresourceRange.baseArrayLayer = 0;
                createInfo.subresourceRange.layerCount = 1;
                if(vkCreateImageView(m_device, &createInfo, nullptr, &ds.imageViews[i]) != VK_SUCCESS) {
                    yCritical() << "Failed to create one of the Swap Image Viewers!";
                }
            }
            
            //  ----------------------------
            //      FRAME BUFFER
            //  ----------------------------

            ds.frameBuffers.resize(ds.imageViews.size(), nullptr);
            for (size_t i = 0; i < ds.imageViews.size(); i++) {
                VkImageView attachments[] = {
                    ds.imageViews[i]
                };

                VqFramebufferCreateInfo framebufferInfo;
                framebufferInfo.renderPass = m_renderPass;
                framebufferInfo.attachmentCount = 1;
                framebufferInfo.pAttachments = attachments;
                framebufferInfo.width = ds.extents.width;
                framebufferInfo.height = ds.extents.height;
                framebufferInfo.layers = 1;

                if (vkCreateFramebuffer(m_device, &framebufferInfo, nullptr, &ds.frameBuffers[i]) != VK_SUCCESS) {
                    yCritical() << "Failed to create framebuffer!";
                    return false;
                }
            }

            //  ----------------------------
            //      COMMAND BUFFER
            //  ----------------------------

            VqCommandBufferAllocateInfo allocInfo;
            allocInfo.commandPool = m_commandPool;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandBufferCount = 1;

            if (vkAllocateCommandBuffers(m_device, &allocInfo, &ds.commandBuffer) != VK_SUCCESS) {
                yError () << "Failed to allocate command buffers!";
                return false;
            }

            return true;            
        }


        bool VqWindow::init_descriptor_pool(const WindowCreateInfo&i)
        {
            if(m_descriptorPool)
                return true;
            m_descriptorCount   = std::max(kMinimumDescriptors,i.descriptors);
            std::vector<VkDescriptorPoolSize> pool_sizes =
            {
                { VK_DESCRIPTOR_TYPE_SAMPLER, m_descriptorCount },
                { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, m_descriptorCount },
                { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, m_descriptorCount },
                { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, m_descriptorCount },
                { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, m_descriptorCount },
                { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, m_descriptorCount },
                { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, m_descriptorCount },
                { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, m_descriptorCount },
                { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, m_descriptorCount },
                { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, m_descriptorCount },
                { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, m_descriptorCount }
            };
            VqDescriptorPoolCreateInfo pool_info;
            pool_info.flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            pool_info.maxSets       = m_descriptorCount * pool_sizes.size();
            pool_info.poolSizeCount = (uint32_t) pool_sizes.size();
            pool_info.pPoolSizes    = pool_sizes.data();
            if(vkCreateDescriptorPool(m_device, &pool_info, nullptr, &m_descriptorPool) != VK_SUCCESS){
                yError() << "Unable to allocate the descriptor pool!";
                return false;
            }
            return true;
        }

        bool VqWindow::init(const WindowCreateInfo& i)
        {
            VkInstance     inst  = VqApp::instance();
            if(!inst){
                yCritical() << "Vulkan has not been initialized!";
                return false;
            }
            
            auto    auto_kill   = safety([this](){ kill(); });

            if(!init_physical(i))
                return false;
            if(!init_window(i))
                return false;
            if(!init_surface())
                return false;
            if(!init_logical())
                return false;
            

                //  EVENTUALLY ENCAPSULATE THE FOLLOWING....
            
            auto    pmodes          = make_set(vqGetPhysicalDeviceSurfacePresentModesKHR(m_physical, m_surface));
            
            //for(auto& sf : vqGetPhysicalDeviceSurfaceFormatsKHR(m_physical, m_surface)){
                //yInfo() << "Format available... " << to_string(sf.format) << "/" << to_string(sf.colorSpace);
            //}

            //  cache details.....
        
            m_present.mode          = pmodes.contains(i.pmode) ? i.pmode : VK_PRESENT_MODE_FIFO_KHR;
                // I know this is available on my card, get smarter later.
            m_surfaceFormat         = VK_FORMAT_B8G8R8A8_SRGB;
            m_surfaceColorSpace     = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
                
            if(!init_command_pool())
                return false;
            if(!init_render_pass())
                return false;
            if(!init_sync())
                return false;
            if(!init_descriptor_pool(i))
                return false;
            if(!init(m_dynamic))
                return false;
            
            set_clear(i.clear);
            auto_kill.clear();  // disarm the safety kill
            return true;
        }


        void VqWindow::kill(DynamicStuff&ds)
        {
            ds.commandBuffer  = nullptr;
            for (VkFramebuffer fb : ds.frameBuffers) {
                if(fb)
                    vkDestroyFramebuffer(m_device, fb, nullptr);
            }
            ds.frameBuffers.clear();
            for(auto iv : ds.imageViews){
                if(iv)
                    vkDestroyImageView(m_device, iv, nullptr);
            }
            ds.imageViews.clear();
            if(ds.swapChain){
                vkDestroySwapchainKHR(m_device, ds.swapChain, nullptr);
                ds.swapChain  = nullptr;
            }
        }

        void VqWindow::kill()
        {
            if(m_device){
                vkDeviceWaitIdle(m_device);
            }
            
            kill(m_dynamic);
            
            if(m_descriptorPool){
                vkDestroyDescriptorPool(m_device, m_descriptorPool, nullptr);
                m_descriptorPool   = nullptr;
            }
            if(m_imageAvailableSemaphore){
                vkDestroySemaphore(m_device, m_imageAvailableSemaphore, nullptr);
                m_imageAvailableSemaphore  = nullptr;
            }
            if(m_renderFinishedSemaphore){
                vkDestroySemaphore(m_device, m_renderFinishedSemaphore, nullptr);
                m_renderFinishedSemaphore  = nullptr;
            }
            m_inFlightFence   = {};

            if(m_renderPass){
                vkDestroyRenderPass(m_device, m_renderPass, nullptr);
                m_renderPass    = nullptr;
            }
            
            if(m_commandPool){
                vkDestroyCommandPool(m_device, m_commandPool, nullptr);
                m_commandPool    = nullptr;
            }
            
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
            pipelineInfo.renderPass = win->m_renderPass;
            pipelineInfo.subpass = 0;             
            pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
            pipelineInfo.basePipelineIndex = -1; // Optional        

            if (vkCreateGraphicsPipelines(win->m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
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
            renderPassInfo.renderPass = m_renderPass;
            renderPassInfo.framebuffer = m_dynamic.frameBuffers[imageIndex];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = m_dynamic.extents;

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
            m_inFlightFence.wait_reset();
            uint32_t imageIndex = 0;
            vkAcquireNextImageKHR(m_device, m_dynamic.swapChain, UINT64_MAX, m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
            vkResetCommandBuffer(m_dynamic.commandBuffer, 0);
            
            if(!record(m_dynamic.commandBuffer, imageIndex))
                return false;
            
            VqSubmitInfo submitInfo;

            VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphore};
            VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = waitSemaphores;
            submitInfo.pWaitDstStageMask = waitStages;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &m_dynamic.commandBuffer;

            VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphore};
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = signalSemaphores;

            if (vkQueueSubmit(m_graphics.queue, 1, &submitInfo, m_inFlightFence) != VK_SUCCESS) {
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
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = &m_dynamic.swapChain;
            presentInfo.pImageIndices = &imageIndex;
            presentInfo.pResults = nullptr; // Optional
            vkQueuePresentKHR(m_present.queue, &presentInfo);
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
            return m_dynamic.commandBuffer;
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
            return m_renderPass;
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
            return m_dynamic.def_scissor();
        }
        
        VkViewport  VqWindow::swap_def_viewport() const
        {
            return m_dynamic.def_viewport();
        }

        uint32_t    VqWindow::swap_image_count() const
        {
            return m_dynamic.imageCount;
        }

        uint32_t    VqWindow::swap_height() const
        {
            return m_dynamic.extents.height;
        }

        uint32_t    VqWindow::swap_min_image_count() const
        {
            return m_dynamic.minImageCount;
        }

        uint32_t    VqWindow::swap_width() const
        {
            return m_dynamic.extents.width;
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

        VkRect2D    VqWindow::DynamicStuff::def_scissor() const
        {
            VkRect2D    ret{};
            ret.offset  = { 0, 0 };
            ret.extent  = extents;
            return ret;
        }
        
        VkViewport  VqWindow::DynamicStuff::def_viewport() const
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
        
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////

        YQ_INVOKE(
            writer<VqWindow>();
        )
    }

}
