////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

/*
    YES, we're leveraging the "hello triangle" tutorial
    
    https://vulkan-tutorial.com/
*/

#include "Application.hpp"
#include "Window.hpp"

#include <engine/vulqan/VqException.hpp>
#include <engine/vulqan/VqLogging.hpp>
#include <engine/vulqan/VqShaderStages.hpp>
#include <engine/vulqan/VqUtils.hpp>

#include <basic/CollectionUtils.hpp>
#include <basic/Logging.hpp>
#include <basic/Safety.hpp>
#include <basic/meta/ObjectInfoWriter.hpp>
#include <engine/pipeline/PipelineConfig.hpp>
#include <math/shape/Size2.hpp>
#include <math/vec/Vector2.hpp>

#include <algorithm>
#include <cassert>

#include <GLFW/glfw3.h>

YQ_OBJECT_IMPLEMENT(yq::engine::Window)

namespace yq {
    namespace engine {
        void Window::poll_events()
        {
            glfwPollEvents();
        }
        
        WindowInfo::WindowInfo(std::string_view kname, ObjectInfo& base, const std::source_location& sl) :
            ObjectInfo(kname, base, sl)
        {
        }

        ////////////////////////////////////////////////////////////////////////////////

        void Window::callback_resize(GLFWwindow* gwin, int, int)
        {
            Window    *v  = (Window*) glfwGetWindowUserPointer(gwin);
            if(v)
                v -> m_rebuildSwap    = true;
        }

        ////////////////////////////////////////////////////////////////////////////////


        Window::Window(const WindowCreateInfo&i)
        {
            init(i);
        }
        
        Window::~Window()
        {
            kill();
        }

        bool Window::init_physical(const WindowCreateInfo& i)
        {
            if(m_physical)
                return true;
        
            m_physical  = i.device;
            if(!m_physical){
                m_physical  = vqFirstDevice();
                if(!m_physical){
                    vqCritical << "Cannot create window without any devices!";
                    return false;
                }
            }
            return true;
        }

        bool Window::init_logical()
        {
            if(m_device)
                return true;    // already initialized!
                
            std::vector<const char*> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };

            auto queueInfos         = vqFindQueueFamilies(m_physical, m_surface);
            if(!queueInfos.graphics.has_value()){
                vqCritical << "Unable to get a queue with graphic capability!";
                return false;
            }
            if(!queueInfos.present.has_value()){
                vqCritical << "Unable to find a present queue!";
                return false;
            }
        
            std::vector<VkDeviceQueueCreateInfo> qci;
            float quePri    = 1.0f;
            uint32_t    qgf  = queueInfos.graphics.value();
            uint32_t    qpf  = queueInfos.present.value();

            for(uint32_t i : std::set<uint32_t>({ qgf, qpf })){
                if(i == UINT32_MAX)
                    continue;
            
                VqDeviceQueueCreateInfo info;
                info.queueFamilyIndex = i;
                info.queueCount = 1;
                info.pQueuePriorities    = &quePri;
                qci.push_back(info);
            }

            const Application*   app    = Application::app();
            if(!app){
                vqCritical << "Unintialized or no application present!";
                return false;
            }
        
            VkPhysicalDeviceFeatures    df{};
            df.fillModeNonSolid         = VK_TRUE;

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
                vqCritical << "Unable to create logical device!";
                return false;
            }
            
                /*
                    Graphics queues
                */
            m_graphics  = VqQueues(m_device, qgf, 1 );
            m_present   = VqQueues(m_device, qpf, 1 );
            return true;
        }
        

        bool Window::init_render_pass()
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
                vqCritical << "Unable to create the render pass!";
                return false;
            } 

            return true;
        }
        
        bool Window::init_sync()
        {
            bool    success = true;
            if(!m_imageAvailableSemaphore){
                VqSemaphoreCreateInfo   semaphoreInfo;
                if(vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS){
                    vqCritical << "Unable to create semaphore for available images!";
                    success = false;
                }
            }
            if(!m_renderFinishedSemaphore){
                VqSemaphoreCreateInfo   semaphoreInfo;
                if(vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphore) != VK_SUCCESS){
                    vqCritical << "Unable to create semaphore for finished rendering!";
                    success = false;
                }
            }
            
            if(!m_inFlightFence){
                m_inFlightFence   = VqFence(*this);
                if(!m_inFlightFence.good()){
                    vqError << "Failed to create fence!";
                    success = false;
                }
            }
            
            return success;
            
        }
        

        bool Window::init(DynamicStuff&ds, VkSwapchainKHR old)
        {
            //  ----------------------------
            //      SWAP CHAIN       
            //  ----------------------------
            
            VkSurfaceCapabilitiesKHR    capabilities;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physical, m_surface, &capabilities);
            if (capabilities.currentExtent.width == std::numeric_limits<uint32_t>::max()) {
                ds.extents = capabilities.currentExtent;
            } else {
                int w, h;
                glfwGetFramebufferSize(m_window, &w, &h);
                ds.extents = {};
                ds.extents.width    = std::clamp((uint32_t) w, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
                ds.extents.height   = std::clamp((uint32_t) h, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
            }

            #if 0
            int w, h;
            glfwGetFramebufferSize(m_window, &w, &h);
            vqInfo << "init dymamic stuff\n"<<
            "Frame itself is [" << w << 'x' << h << "] vs\n" <<
            "Image extents is " << ds.extents << '\n' <<
            "Cur is " << capabilities.currentExtent << '\n' <<
            "Min is " << capabilities.minImageExtent << '\n' <<
            "Max is " << capabilities.maxImageExtent
            ;
            #endif

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
            
            uint32_t queueFamilyIndices[] = {m_graphics.family(), m_present.family()};
            if (m_graphics.family() != m_present.family()) {
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
            createInfo.presentMode      = m_presentMode;
            createInfo.clipped          = VK_TRUE;
            
                // TEMPORARY UNTIL WE GET THE NEW ONE
            createInfo.oldSwapchain = old;
            
            if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &ds.swapChain) != VK_SUCCESS){
                ds.swapChain   = nullptr;
                vqCritical << "Failed to create the SWAP chain!";
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
                    vqCritical << "Failed to create one of the Swap Image Viewers!";
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
                    vqCritical << "Failed to create framebuffer!";
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
                vqError << "Failed to allocate command buffers!";
                return false;
            }

            return true;            
        }


        bool Window::init_descriptor_pool(const WindowCreateInfo&i)
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
                vqError << "Unable to allocate the descriptor pool!";
                return false;
            }
            return true;
        }

        bool Window::init(const WindowCreateInfo& i)
        {
            try {
                VkInstance     inst  = Application::vulkan();
                if(!inst){
                    vqCritical << "Vulkan has not been initialized!";
                    return false;
                }
                
//                auto    auto_kill   = safety([this](){ kill(); });

                if(!init_physical(i))
                    return false;

                m_window    = VqWindow(this, i);
                m_surface   = VqSurface(m_physical, m_window);

                if(!init_logical())
                    return false;
                

                    //  EVENTUALLY ENCAPSULATE THE FOLLOWING....
                
                auto    pmodes          = make_set(vqGetPhysicalDeviceSurfacePresentModesKHR(m_physical, m_surface));
                
                //for(auto& sf : vqGetPhysicalDeviceSurfaceFormatsKHR(m_physical, m_surface)){
                    //vqInfo << "Format available... " << to_string(sf.format) << "/" << to_string(sf.colorSpace);
                //}

                //  cache details.....
            
                m_presentMode          = m_surface.supports(i.pmode) ? i.pmode : VK_PRESENT_MODE_FIFO_KHR;
                    // I know this is available on my card, get smarter later.
                m_surfaceFormat         = VK_FORMAT_B8G8R8A8_SRGB;
                m_surfaceColorSpace     = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
                    
                m_commandPool           = VqCommandPool(m_device, m_graphics.family());
                
                if(!init_render_pass())
                    return false;
                if(!init_sync())
                    return false;
                if(!init_descriptor_pool(i))
                    return false;
                if(!init(m_dynamic))
                    return false;
                
                set_clear(i.clear);
//                auto_kill.clear();  // disarm the safety kill
                return true;
            } 
            catch(VqException& ex){
                yCritical() << ex.what();
            }
            kill();
            return false;
        }


        void Window::kill(DynamicStuff&ds)
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

        void Window::kill()
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
            
            m_commandPool   = {};
            m_graphics      = {};
            m_present       = {};
            
            if(m_device){
                vkDestroyDevice(m_device, nullptr);
                m_device   = nullptr;
            }
            
            m_surface       = {};
            m_window        = {};
            
            m_physical = nullptr;
        }

        ////////////////////////////////////////////////////////////////////////////////

        bool    Window::Pipeline::init(Window*win, const PipelineConfig&cfg)
        {
            VqShaderStages stages(*win, cfg.shaders);

            VqPipelineVertexInputStateCreateInfo    vertexInfo;
            
            std::vector<VkVertexInputAttributeDescription>  attrs;
            std::vector<VkVertexInputBindingDescription>    vbos;
            
            for(uint32_t i=0;i<cfg.vbos.size();++i){
                auto& v = cfg.vbos[i];
                VkVertexInputBindingDescription b;
                b.binding   = i;
                b.stride    = v.stride;
                b.inputRate = (VkVertexInputRate) v.inputRate.value();
                vbos.push_back(b);
                
                for(auto& va : v.attrs){
                    VkVertexInputAttributeDescription   a;
                    a.binding       = i;
                    a.location      = va.location;
                    a.offset        = va.offset;
                    a.format        = (VkFormat) va.format.value();
                    attrs.push_back(a);
                }
            }

            vertexInfo.vertexBindingDescriptionCount    = (uint32_t) vbos.size();
            vertexInfo.pVertexBindingDescriptions       = vbos.data();
            vertexInfo.vertexAttributeDescriptionCount  = (uint32_t) attrs.size();
            vertexInfo.pVertexAttributeDescriptions     = attrs.data();
            
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
            rasterizer.cullMode = (VkCullModeFlags) cfg.culling.value();
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
                vqError << "Failed to create pipeline layout!";
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
            
            if(cfg.polymode == PolygonMode::Fill){
                pipelineInfo.flags  = VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT;
            } 
            if (vkCreateGraphicsPipelines(win->m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
                vqError << "Failed to create graphics pipeline!";
                return false;
            }
            
                // if it's a fill polygon (typical), create a derivative wireframe pipeline
            if(cfg.polymode == PolygonMode::Fill){
                pipelineInfo.flags  = VK_PIPELINE_CREATE_DERIVATIVE_BIT;
                pipelineInfo.basePipelineHandle = pipeline;
                pipelineInfo.basePipelineIndex  = -1;
                rasterizer.polygonMode  = VK_POLYGON_MODE_LINE;
                
                if (vkCreateGraphicsPipelines(win->m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &wireframe) != VK_SUCCESS) {
                    vqError << "Failed to create wireframe pipeline!";
                    return false;
                }
            }
            return true;
        }
        
        void    Window::Pipeline::kill(Window*win)
        {
            if(wireframe){
                vkDestroyPipeline(win->m_device, wireframe, nullptr);
                wireframe    = nullptr;
            }
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

        bool    Window::record(VkCommandBuffer commandBuffer, uint32_t imageIndex)
        {
            VqCommandBufferBeginInfo beginInfo;
            beginInfo.flags = 0; // Optional
            beginInfo.pInheritanceInfo = nullptr; // Optional

            if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
                vqError << "Failed to begin recording command buffer!";
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
                vqError << "Failed to record command buffer!";
                return false;
            }
            return true;
        }
        
        bool    Window::draw()
        {
            bool    rebuild = m_rebuildSwap.exchange(false);
            if(rebuild){
                vkDeviceWaitIdle(m_device);
                DynamicStuff        newStuff;
                init(newStuff, m_dynamic.swapChain);
                std::swap(m_dynamic, newStuff);
                kill(newStuff);
                
                //  resize notifications...
            }
        
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

            if (vkQueueSubmit(m_graphics[0], 1, &submitInfo, m_inFlightFence) != VK_SUCCESS) {
                vqError << "Failed to submit draw command buffer!";
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
            vkQueuePresentKHR(m_present[0], &presentInfo);
            return true;
        }
        
        
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////

        void Window::attention()
        {
            if(m_window)
                glfwRequestWindowAttention(m_window);
        }

        void Window::close()
        {
            if(m_window)
                glfwSetWindowShouldClose(m_window, GLFW_TRUE);
        }

        VkCommandBuffer     Window::command_buffer() const
        {
            return m_dynamic.commandBuffer;
        }

        void Window::focus()
        {
            if(m_window)
                glfwFocusWindow(m_window);
        }

        VkQueue     Window::graphics_queue() const 
        { 
            return m_graphics[0]; 
        }
        
        uint32_t    Window::graphics_queue_family() const 
        { 
            return m_graphics.family(); 
        }

        int  Window::height() const
        {
            if(!m_window)
                return 0;
        
            int ret;
            glfwGetWindowSize(m_window, nullptr, &ret);
            return ret;
        }

        void Window::hide()
        {
            if(m_window)
                glfwHideWindow(m_window);
        }

        void Window::iconify()
        {
            if(m_window)
                glfwIconifyWindow(m_window);
        }

        bool        Window::is_decorated() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_DECORATED) != 0;
        }
        
        bool        Window::is_focused() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_FOCUSED ) != 0;
        }
        
        bool        Window::is_floating() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_FLOATING) != 0;
        }
        
        bool        Window::is_fullscreen() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowMonitor(m_window) != nullptr;
        }
        
        bool        Window::is_hovered() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_HOVERED) != 0;
        }
        
        bool        Window::is_iconified() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0;
        }

        bool        Window::is_maximized() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_MAXIMIZED) != 0;
        }
        
        bool        Window::is_resizable() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_RESIZABLE) != 0;
        }
        
        bool        Window::is_visible() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_VISIBLE) != 0;
        }
        

        void        Window::maximize()
        {
            if(m_window)
                glfwMaximizeWindow(m_window);
        }

        VqMonitor   Window::monitor() const
        {
            if(m_window)
                return VqMonitor(glfwGetWindowMonitor(m_window));
            return VqMonitor();
        }

        Vector2I    Window::position() const
        {
            if(!m_window)
                return {};
            Vector2I   ret;
            glfwGetWindowPos(m_window, &ret.x, &ret.y);
            return ret;
        }


        VkRenderPass Window::render_pass() const
        {
            return m_renderPass;
        }

        void        Window::restore()
        {
            if(m_window)
                glfwRestoreWindow(m_window);
        }

        void        Window::set_clear(const ColorRgbaF&i)
        {
            m_clear = VkClearValue{{{ i.red, i.green, i.blue, i.alpha }}};
        }

        void        Window::set_position(const Vector2I& pos)
        {
            set_position(pos.x, pos.y);
        }
        
        void        Window::set_position(int x, int y)
        {
            if(m_window){
                glfwSetWindowPos(m_window, x, y);
            }
        }

        void        Window::set_size(const Size2I& sz)
        {
            set_size(sz.x, sz.y);
        }

        void        Window::set_size(int w, int h)
        {
            if(m_window){
                glfwSetWindowSize(m_window, std::max(1, w), std::max(1, h));
            }
        }

        void        Window::set_title(const char*z)
        {
            if(m_window && z){
                glfwSetWindowTitle(m_window, z);
            }
        }

        void        Window::set_title(const std::string&z)
        {
            set_title(z.c_str());
        }

        bool        Window::should_close() const
        {
            if(!m_window) 
                return true;
            return glfwWindowShouldClose(m_window);
        }

        void        Window::show()
        {
            if(m_window)
                glfwShowWindow(m_window);
        }

        Size2I      Window::size() const
        {
            if(!m_window)
                return {};
            Size2I  ret;
            glfwGetWindowSize(m_window, &ret.x, &ret.y);
            return ret;
        }

        VkRect2D    Window::swap_def_scissor() const
        {
            return m_dynamic.def_scissor();
        }
        
        VkViewport  Window::swap_def_viewport() const
        {
            return m_dynamic.def_viewport();
        }

        uint32_t    Window::swap_image_count() const
        {
            return m_dynamic.imageCount;
        }

        uint32_t    Window::swap_height() const
        {
            return m_dynamic.extents.height;
        }

        uint32_t    Window::swap_min_image_count() const
        {
            return m_dynamic.minImageCount;
        }

        uint32_t    Window::swap_width() const
        {
            return m_dynamic.extents.width;
        }
        
        int  Window::width() const
        {
            if(!m_window)
                return 0;
        
            int ret;
            glfwGetWindowSize(m_window, &ret, nullptr);
            return ret;
        }
        
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////

        VkRect2D    Window::DynamicStuff::def_scissor() const
        {
            VkRect2D    ret{};
            ret.offset  = { 0, 0 };
            ret.extent  = extents;
            return ret;
        }
        
        VkViewport  Window::DynamicStuff::def_viewport() const
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
            writer<Window>();
        )
    }

}
