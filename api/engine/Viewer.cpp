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
#include "Perspective.hpp"
#include "Scene.hpp"
#include "Viewer.hpp"

#include <basic/CollectionUtils.hpp>
#include <basic/Logging.hpp>
#include <basic/Safety.hpp>
#include <basic/TextUtils.hpp>
#include <basic/meta/ObjectInfoWriter.hpp>

#include <engine/render/IndexBufferObjectInfo.hpp>
#include <engine/render/Pipeline.hpp>
#include <engine/render/PipelineConfig.hpp>
#include <engine/render/PushBufferObjectInfo.hpp>
#include <engine/render/Rendered.hpp>
#include <engine/render/Render3D.hpp>
#include <engine/render/StdPushConstant.hpp>
#include <engine/render/UniformBufferObjectInfo.hpp>
#include <engine/render/VertexBufferObjectInfo.hpp>

#include <engine/Visualizer.hpp>

#include <engine/vulqan/VqBuffer.hpp>
#include <engine/vulqan/VqException.hpp>
#include <engine/vulqan/VqLogging.hpp>
#include <engine/vulqan/VqPipeline.hpp>
#include <engine/vulqan/VqShaderStages.hpp>
#include <engine/vulqan/VqStructs.hpp>
#include <engine/vulqan/VqUtils.hpp>

#include <math/Size2.hpp>
#include <math/Vector2.hpp>

#include <algorithm>
#include <cassert>

#include <GLFW/glfw3.h>

#include <basic/Logging.hpp>
#include <engine/Application.hpp>
#include <engine/Viewer.hpp>
#include <engine/render/Pipeline.hpp>

YQ_OBJECT_IMPLEMENT(yq::engine::Viewer)

namespace yq {
    namespace engine {
        void Viewer::poll_events()
        {
            glfwPollEvents();
        }
        
        WindowInfo::WindowInfo(std::string_view kname, ObjectInfo& base, const std::source_location& sl) :
            ObjectInfo(kname, base, sl)
        {
        }

        ////////////////////////////////////////////////////////////////////////////////

        Viewer::Viewer(const ViewerCreateInfo&i)
        {
            try {
                m           = std::make_unique<Visualizer>(i,this);
            }
            catch(VqException& ex){
                yCritical() << ex.what();
            }
            set_clear_color(i.clear);
            m_title     = i.title;
            yNotice() << "Using (" << to_string(gpu_type()) << "): " << gpu_name();
        }
        
        Viewer::~Viewer()
        {
            m           = nullptr;
        }



        ////////////////////////////////////////////////////////////////////////////////

        bool    Viewer::record(VkCommandBuffer commandBuffer, uint32_t imageIndex)
        {
            VqCommandBufferBeginInfo beginInfo;
            beginInfo.flags = 0; // Optional
            beginInfo.pInheritanceInfo = nullptr; // Optional

            if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
                vqError << "Failed to begin recording command buffer!";
                return false;
            }

            VqRenderPassBeginInfo renderPassInfo;
            renderPassInfo.renderPass = m->m_renderPass;
            renderPassInfo.framebuffer = m->dynamic.frameBuffers[imageIndex];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = m->dynamic.swapchain.extents();

            renderPassInfo.clearValueCount = 1;
            VkClearValue                cv  = m->m_clearValue;
            renderPassInfo.pClearValues = &cv;
            vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        
            draw_vulqan(commandBuffer);

            vkCmdEndRenderPass(commandBuffer);

            if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
                vqError << "Failed to record command buffer!";
                return false;
            }
            return true;
        }
        
        bool    Viewer::draw()
        {
            ++m->tick;      // frame
            auto start  = std::chrono::high_resolution_clock::now();
            
            bool    rebuild = m->m_rebuildSwap.exchange(false);
            if(rebuild){
                vkDeviceWaitIdle(m->m_device);
                VqDynamic        newStuff;
                m->init(newStuff, m->dynamic.swapchain);
                std::swap(m->dynamic, newStuff);
                m->kill(newStuff);
                
                //  resize notifications...
            }
            
            
            VkCommandBuffer cmdbuf = m->dynamic.commandBuffers[0];
            m->inFlightFence.wait_reset();

            uint32_t imageIndex = 0;
            vkAcquireNextImageKHR(m->m_device, m->dynamic.swapchain, UINT64_MAX, m->imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
            vkResetCommandBuffer(cmdbuf, 0);
            
            if(!record(cmdbuf, imageIndex))
                return false;
            
            VqSubmitInfo submitInfo;

            VkSemaphore waitSemaphores[] = {m->imageAvailableSemaphore};
            VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores      = waitSemaphores;
            submitInfo.pWaitDstStageMask    = waitStages;
            submitInfo.commandBufferCount   = (uint32_t) m->dynamic.commandBuffers.size();
            submitInfo.pCommandBuffers      = m->dynamic.commandBuffers.data();

            VkSemaphore signalSemaphores[] = {m->renderFinishedSemaphore};
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = signalSemaphores;

            if (vkQueueSubmit(graphic_queue(), 1, &submitInfo, m->inFlightFence) != VK_SUCCESS) {
                vqError << "Failed to submit draw command buffer!";
                return false;
            }
                
            VqPresentInfoKHR presentInfo;
            
            VkSwapchainKHR      swapchain   = m->dynamic.swapchain;

            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = signalSemaphores;
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = &swapchain;
            presentInfo.pImageIndices = &imageIndex;
            presentInfo.pResults = nullptr; // Optional
            vkQueuePresentKHR(present_queue(), &presentInfo);
            
            auto end = std::chrono::high_resolution_clock::now();
            m->draw_time    = (end-start).count();
            return true;
        }

        void    Viewer::render(VkCommandBuffer buf, const Scene& scene, const Perspective& pr)
        {   
            if(!pr.camera){
                yCritical() << "Camera not detected!\n";
                return ;
            }
    
            CameraParams        cparams;
            cparams.screen      = (Size2D) size();
            glm::dmat4          cam_matrix = pr.camera->world2screen(cparams);
            
            VkPipeline          prev_pipeline   = nullptr;
            StdPushConstant     stdPush;
            
            for(const Rendered* r : scene.things){
                const Pipeline* pipe    = r->pipeline();
                if(!pipe){
                    continue;
                }
                    
                const Render3D *r3  = dynamic_cast<const Render3D*>(r);

                ViPipeline*     vp      = nullptr;
                ViObject*       vo      = nullptr;
                bool            pnew      = false;
                bool            onew      = false;
                
                std::tie( vp, pnew )    = m->pipeline(pipe->id());
                std::tie( vo, onew )    = m->object(r->id());
                
                if(pnew)
                    vp->vq        = std::make_unique<VqPipeline>(*m, pipe->config());
                
                uint32_t            shader_mask = vp->vq->shader_mask();
                Tristate            wireframe   = (r->wireframe() == Tristate::INHERIT) ? pr.wireframe : r->wireframe();
                VkPipeline          vkpipe  = (wireframe == Tristate::YES) ? vp->vq->wireframe() : vp->vq->pipeline();
                VkPipelineLayout    vklay   = vp->vq->layout();
            
                //  =================================================
                //      SET THE PIPELINE
            
                if(vkpipe && (vkpipe != prev_pipeline)){
                    vkCmdBindPipeline(buf, VK_PIPELINE_BIND_POINT_GRAPHICS, vkpipe);
                    prev_pipeline   = vkpipe;
                }
                
                //  =================================================
                //      PUSH THE CONSTANTS

                switch(pipe->push_type()){
                case PushConfigType::Full:
                    if(r3){
                        stdPush.matrix  = cam_matrix * r3->model2world();
                        vkCmdPushConstants(buf, vklay, shader_mask, 0, sizeof(stdPush), &stdPush );
                    }
                    break;
                case PushConfigType::View:
                    stdPush.matrix  = cam_matrix;
                    vkCmdPushConstants(buf, vklay, shader_mask, 0, sizeof(stdPush), &stdPush );
                    break;
                case PushConfigType::Custom:
                    {
                        const PushBufferObjectInfo* pbo = pipe->push_buffer();
                        if(pbo){
                            BufferData      bd  = pbo->get(r);
                            if(bd.data && bd.size)
                                vkCmdPushConstants(buf, vklay, shader_mask, 0, bd.size, bd.data);
                        }
                    }
                    break;
                case PushConfigType::None:
                default:
                    // no push constant desired... which is fine.
                    break;
                }
                
                //  =================================================
                //      VERTEX BUFFERS
                
                //std::vector<VkBuffer>       buffers;
                //std::vector<VkDeviceSize>   offsets;
                
                for(const VertexBufferObjectInfo* vbo : pipe->vertex_buffers()){
                    if(!vbo) [[unlikely]]
                        continue;
                    ViBuffer*   buffer  = nullptr;
                    Tristate    update;

                    DataActivity    da  = vbo->data_activity();
                    bool            check   = false;
                    bool            fast    = false;
                    bool            bnew    = false;
                    switch(da){
                    case DataActivity::UNSURE:
                        continue;
                    case DataActivity::COMMON:
                        std::tie(buffer, bnew)  = vp -> buffer(vbo->id());
                        break;
                    case DataActivity::STATIC:
                        std::tie(buffer, bnew)  = vo -> buffer(vbo->id());
                        break;
                    case DataActivity::DYNAMIC:
                        std::tie(buffer, bnew)  = vo -> buffer(vbo->id());
                        check   = true;
                        break;
                    case DataActivity::REFRESH:
                        std::tie(buffer, bnew)  = vo -> buffer(vbo->id());
                        check   = true;
                        fast    = true;
                        break;
                    }
                    
                    
                    if(bnew || check){
                        BufferData  bd    = vbo->get(r);
                        if(!bd.data || !bd.size)
                            continue;
                        if(bnew || fast || (check && bd.revision != buffer->rev)){
                            // optimize this ... later
                            buffer -> vq    = std::make_unique<VqBuffer>(*m, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, bd.data, bd.size);
                            buffer -> rev   = bd.revision;
                        }
                    }
                    
                    if(buffer->vq && buffer->vq->buffer()){
                        VkBuffer        vb  = buffer->vq->buffer();
                        VkDeviceSize    zero{};
                        vkCmdBindVertexBuffers(buf, vbo->binding(), 1, &vb, &zero);
                    }
                }
                
                bool    indexed = false;
                size_t  idx_count   = 0;
                
                //  =================================================
                //      INDEX BUFFERS
                for(const IndexBufferObjectInfo* ibo : pipe->index_buffers()){
                    if(!ibo) [[ unlikely ]]
                        continue;
                        
                    if(ibo->index_type() == IndexType::none)[[ unlikely ]]
                        continue;
                    ViBuffer*   buffer  = nullptr;
                    Tristate    update;

                    DataActivity    da  = ibo->data_activity();
                    bool            check   = false;
                    bool            fast    = false;
                    bool            bnew    = false;
                    switch(da){
                    case DataActivity::UNSURE:
                        continue;
                    case DataActivity::COMMON:
                        std::tie(buffer, bnew)  = vp -> buffer(ibo->id());
                        break;
                    case DataActivity::STATIC:
                        std::tie(buffer, bnew)  = vo -> buffer(ibo->id());
                        break;
                    case DataActivity::DYNAMIC:
                        std::tie(buffer, bnew)  = vo -> buffer(ibo->id());
                        check   = true;
                        break;
                    case DataActivity::REFRESH:
                        std::tie(buffer, bnew)  = vo -> buffer(ibo->id());
                        check   = true;
                        fast    = true;
                        break;
                    }
                    
                    
                    if(bnew || check){
                        BufferData  bd    = ibo->get(r);
                        if(!bd.data || !bd.size)
                            continue;
                        if(bnew || fast || (check && bd.revision != buffer->rev)){
                            // optimize this ... later
                            buffer -> vq    = std::make_unique<VqBuffer>(*m, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, bd.data, bd.size);
                            buffer -> rev   = bd.revision;
                        }
                    }
                    
                    if(buffer->vq && buffer->vq->buffer()){
                        VkBuffer        vb  = buffer->vq->buffer();
                        
                        // recover the size... (yeah, short-sighted...)
                        switch(ibo->index_type()){
                        case IndexType::uint32:
                            idx_count   = buffer->vq->size() / sizeof(uint32_t);
                            break;
                        case IndexType::uint16:
                            idx_count   = buffer->vq->size() / sizeof(uint16_t);
                            break;
                        case IndexType::uint8:
                            idx_count   = buffer->vq->size();
                            break;
                        default:
                            break;
                        }
                        vkCmdBindIndexBuffer(buf, vb, 0, (VkIndexType)(ibo->index_type().value()));
                        indexed         = true;
                        break;
                    }                    
                }

                //if(!buffers.empty())
                    //vkCmdBindVertexBuffers(buf, 0, buffers.size(), buffers.data(), offsets.data());
             
                const auto& dc = r->draw();
                if(indexed){
                    vkCmdDrawIndexed(buf, idx_count, 1, 0, 0, 0);
                } else {
                    vkCmdDraw(buf, dc.vertex_count, 1, 0, 0);
                }
            }
        }        
        
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////


        void Viewer::attention()
        {
            if(m->m_window)
                glfwRequestWindowAttention(m->m_window);
        }

        RGBA4F Viewer::clear_color() const
        {
            VkClearValue    cv  = m->m_clearValue;
            return { 
                cv.color.float32[0], cv.color.float32[1], 
                cv.color.float32[2], cv.color.float32[3] 
            };
        }

        void Viewer::close()
        {
            if(m->m_window)
                glfwSetWindowShouldClose(m->m_window, GLFW_TRUE);
        }


        VkCommandBuffer  Viewer::command_buffer() const
        {
            return m->dynamic.commandBuffers[0];
        }

        VkCommandPool  Viewer::command_pool() const 
        { 
            return m->m_thread->graphic; 
        }
        
        VkQueue  Viewer::compute_queue(uint32_t i) const
        {
            return m->m_compute[i];
        }
        
        uint32_t  Viewer::compute_queue_count() const
        {
            return (uint32_t) m->m_compute.queues.size();
        }
        
        uint32_t  Viewer::compute_queue_family() const
        {
            return m->m_compute.family;
        }

        VkDescriptorPool    Viewer::descriptor_pool() const 
        { 
            return m->m_thread->descriptors; 
        }
        
        VkDevice  Viewer::device() const 
        { 
            return m->m_device; 
        }

        void Viewer::focus()
        {
            if(m && m->m_window)
                glfwFocusWindow(m->m_window);
        }

        std::string_view    Viewer::gpu_name() const
        {
            return std::string_view(m->m_deviceInfo.deviceName, strnlen(m->m_deviceInfo.deviceName, VK_MAX_PHYSICAL_DEVICE_NAME_SIZE));
        }

        VkPhysicalDeviceType    Viewer::gpu_type() const
        {
            return m->m_deviceInfo.deviceType;
        }

        
        VkQueue     Viewer::graphic_queue(uint32_t i) const
        {
            return m->m_graphic[i];
        }
        
        uint32_t    Viewer::graphic_queue_count() const
        {
            return (uint32_t) m->m_graphic.queues.size();
        }
        
        uint32_t    Viewer::graphic_queue_family() const
        {
            return m->m_graphic.family;
        }


        int  Viewer::height() const
        {
            if(!m->m_window)
                return 0;
        
            int ret;
            glfwGetWindowSize(m->m_window, nullptr, &ret);
            return ret;
        }

        void Viewer::hide()
        {
            if(m->m_window)
                glfwHideWindow(m->m_window);
        }

        void Viewer::iconify()
        {
            if(m->m_window)
                glfwIconifyWindow(m->m_window);
        }

        bool        Viewer::is_decorated() const
        {
            if(!m->m_window)
                return false;
            return glfwGetWindowAttrib(m->m_window, GLFW_DECORATED) != 0;
        }
        
        bool        Viewer::is_focused() const
        {
            if(!m->m_window)
                return false;
            return glfwGetWindowAttrib(m->m_window, GLFW_FOCUSED ) != 0;
        }
        
        bool        Viewer::is_floating() const
        {
            if(!m->m_window)
                return false;
            return glfwGetWindowAttrib(m->m_window, GLFW_FLOATING) != 0;
        }
        
        bool        Viewer::is_fullscreen() const
        {
            if(!m->m_window)
                return false;
            return glfwGetWindowMonitor(m->m_window) != nullptr;
        }
        
        bool        Viewer::is_hovered() const
        {
            if(!m->m_window)
                return false;
            return glfwGetWindowAttrib(m->m_window, GLFW_HOVERED) != 0;
        }
        
        bool        Viewer::is_iconified() const
        {
            if(!m->m_window)
                return false;
            return glfwGetWindowAttrib(m->m_window, GLFW_ICONIFIED) != 0;
        }

        bool        Viewer::is_maximized() const
        {
            if(!m->m_window)
                return false;
            return glfwGetWindowAttrib(m->m_window, GLFW_MAXIMIZED) != 0;
        }
        
        bool        Viewer::is_resizable() const
        {
            if(!m->m_window)
                return false;
            return glfwGetWindowAttrib(m->m_window, GLFW_RESIZABLE) != 0;
        }
        
        bool        Viewer::is_visible() const
        {
            if(!m->m_window)
                return false;
            return glfwGetWindowAttrib(m->m_window, GLFW_VISIBLE) != 0;
        }
        
        VkDevice    Viewer::logical() const 
        { 
            return m->m_device; 
        }


        uint32_t    Viewer::max_memory_allocation_count() const  
        { 
            return m->m_deviceInfo.limits.maxMemoryAllocationCount; 
        }
        
        uint32_t    Viewer::max_push_constants_size() const 
        { 
            return m->m_deviceInfo.limits.maxPushConstantsSize; 
        }
        
        uint32_t    Viewer::max_viewports() const 
        { 
            return m->m_deviceInfo.limits.maxViewports; 
        }

        void        Viewer::maximize()
        {
            if(m->m_window)
                glfwMaximizeWindow(m->m_window);
        }

        VqMonitor   Viewer::monitor() const
        {
            if(m->m_window)
                return VqMonitor(glfwGetWindowMonitor(m->m_window));
            return VqMonitor();
        }

        VkPhysicalDevice    Viewer::physical() const 
        { 
            return m->m_physical; 
        }

        Vector2I    Viewer::position() const
        {
            if(!m->m_window)
                return {};
            Vector2I   ret;
            glfwGetWindowPos(m->m_window, &ret.x, &ret.y);
            return ret;
        }

        PresentMode  Viewer::present_mode() const
        {
            return m->m_presentMode;
        }

        const std::set<PresentMode>&     Viewer::present_modes_available() const
        {
            return m->m_presentModes;
        }

        VkQueue      Viewer::present_queue(uint32_t i) const
        {
            return m->m_present[i];
        }
        
        uint32_t     Viewer::present_queue_count() const
        {
            return (uint32_t) m->m_present.queues.size();
        }
        
        uint32_t     Viewer::present_queue_family() const
        {
            return m->m_present.family;
        }

        VkRenderPass Viewer::render_pass() const
        {
            return m->m_renderPass;
        }

        void        Viewer::restore()
        {
            if(m->m_window)
                glfwRestoreWindow(m->m_window);
        }

        void        Viewer::set_clear_color(const RGBA4F&i)
        {
            m->m_clearValue = VkClearValue{{{ i.red, i.green, i.blue, i.alpha }}};
        }

        void        Viewer::set_position(const Vector2I& pos)
        {
            set_position(pos.x, pos.y);
        }

        void        Viewer::set_present_mode(PresentMode pm)
        {
            if(m->m_presentModes.contains(pm) && (pm != m->m_presentMode)){
                m->m_presentMode    = pm;
                m->m_rebuildSwap    = true;
            }
        }
        
        void        Viewer::set_position(int x, int y)
        {
            if(m->m_window){
                glfwSetWindowPos(m->m_window, x, y);
            }
        }

        void        Viewer::set_size(const Size2I& sz)
        {
            set_size(sz.x, sz.y);
        }

        void        Viewer::set_size(int w, int h)
        {
            if(m->m_window){
                glfwSetWindowSize(m->m_window, std::max(1, w), std::max(1, h));
            }
        }

        void        Viewer::set_title(const char*z)
        {
            if(m->m_window && z){
                glfwSetWindowTitle(m->m_window, z);
                m_title = z;
            }
        }

        void        Viewer::set_title(const std::string&z)
        {
            set_title(z.c_str());
        }

        bool        Viewer::should_close() const
        {
            if(!m->m_window) 
                return true;
            return glfwWindowShouldClose(m->m_window);
        }

        void        Viewer::show()
        {
            if(m->m_window)
                glfwShowWindow(m->m_window);
        }

        Size2I      Viewer::size() const
        {
            if(!m->m_window)
                return {};
            Size2I  ret;
            glfwGetWindowSize(m->m_window, &ret.x, &ret.y);
            return ret;
        }

        bool                Viewer::supports(VkFormat fmt) const
        {
            for(auto& f : m->m_surfaceFormats)
                if(fmt == f.format)
                    return true;
            return false;
        }
        
        bool                Viewer::supports(PresentMode pm) const
        {
            return m->m_presentModes.contains(pm);
        }

        VkSurfaceKHR        Viewer::surface() const 
        { 
            return m->m_surface; 
        }

        VkFormat            Viewer::surface_format() const
        {
            return m->m_surfaceFormat;
        }

        VkColorSpaceKHR  Viewer::surface_color_space() const 
        { 
            return m->m_surfaceColorSpace; 
        }

        VkRect2D    Viewer::swap_def_scissor() const
        {
            return m->dynamic.swapchain.def_scissor();
        }
        
        VkViewport  Viewer::swap_def_viewport() const
        {
            return m->dynamic.swapchain.def_viewport();
        }

        uint32_t    Viewer::swap_image_count() const
        {
            return m->dynamic.imageCount;
        }

        uint32_t    Viewer::swap_height() const
        {
            return m->dynamic.swapchain.height();
        }

        uint32_t    Viewer::swap_min_image_count() const
        {
            return m->dynamic.swapchain.min_image_count();
        }

        uint32_t    Viewer::swap_width() const
        {
            return m->dynamic.swapchain.width();
        }
        
        VkQueue   Viewer::video_decode_queue(uint32_t i) const
        {
            return m->m_videoDecode[i];
        }
        
        uint32_t  Viewer::video_decode_queue_count() const
        {
            return (uint32_t) m->m_videoDecode.queues.size();
        }
        
        uint32_t  Viewer::video_decode_queue_family() const
        {
            return m->m_videoDecode.family;
        }

        VkQueue   Viewer::video_encode_queue(uint32_t i) const
        {
            return m->m_videoEncode[i];
        }
        
        uint32_t  Viewer::video_encode_queue_count() const
        {
            return (uint32_t) m->m_videoEncode.queues.size();
        }

        uint32_t  Viewer::video_encode_queue_family() const
        {
            return m->m_videoEncode.family;
        }

        int  Viewer::width() const
        {
            if(!m->m_window)
                return 0;
        
            int ret;
            glfwGetWindowSize(m->m_window, &ret, nullptr);
            return ret;
        }
        
        GLFWwindow*         Viewer::window() const 
        { 
            return m->m_window; 
        }
        
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////


        ViBuffer::ViBuffer()
        {
        }
        
        ViBuffer::~ViBuffer()
        {
        }
        
        ////////////////////////////////////////////////////////////////////////////////

        ViBufferMap::ViBufferMap()
        {
        }
        
        ViBufferMap::~ViBufferMap()
        {
            for(auto& b : buffers){
                if(b.second)
                    delete b.second;
            }
            buffers.clear();
        }

        ////////////////////////////////////////////////////////////////////////////////

        ViFrame::ViFrame()
        {
        }
        
        ViFrame::~ViFrame()
        {
            dtor();
        }
        
        void    ViFrame::dtor()
        {
        }

        ////////////////////////////////////////////////////////////////////////////////

        ViObject::ViObject()
        {
        }
        
        ViObject::~ViObject()
        {
        }

        ////////////////////////////////////////////////////////////////////////////////

        ViPipeline::ViPipeline()
        {
        }
        
        ViPipeline::~ViPipeline()
        {
        }

        ////////////////////////////////////////////////////////////////////////////////

        void    ViQueues::set(VkDevice dev, uint32_t cnt)
        {
            queues.clear();
            queues.resize(cnt, nullptr);
            for(uint32_t i=0;i<cnt;++i)
                vkGetDeviceQueue(dev, family, i, &queues[i]);
        }
        
        ViQueues::~ViQueues()
        {
            queues.clear();
        }

        VkQueue ViQueues::operator[](uint32_t i) const
        {
            if(i<queues.size()) [[likely]]
                return queues[i];
            return nullptr;
        }


        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////

        ViThread::ViThread(Visualizer& viz) : device(viz.m_device)
        {
            try {
                std::vector<VkDescriptorPoolSize> descriptorPoolSizes =
                {
                    { VK_DESCRIPTOR_TYPE_SAMPLER, viz.m_descriptorCount },
                    { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, viz.m_descriptorCount },
                    { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, viz.m_descriptorCount },
                    { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, viz.m_descriptorCount },
                    { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, viz.m_descriptorCount },
                    { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, viz.m_descriptorCount },
                    { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, viz.m_descriptorCount },
                    { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, viz.m_descriptorCount },
                    { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, viz.m_descriptorCount },
                    { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, viz.m_descriptorCount },
                    { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, viz.m_descriptorCount }
                };
                VqDescriptorPoolCreateInfo descriptorPoolInfo;
                descriptorPoolInfo.flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
                descriptorPoolInfo.maxSets       = viz.m_descriptorCount * descriptorPoolSizes.size();
                descriptorPoolInfo.poolSizeCount = (uint32_t) descriptorPoolSizes.size();
                descriptorPoolInfo.pPoolSizes    = descriptorPoolSizes.data();
                if(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &descriptors) != VK_SUCCESS)
                    throw VqException("Unable to allocate the descriptor pool!");

                VqCommandPoolCreateInfo poolInfo;
                poolInfo.flags              = viz.m_cmdPoolCreateFlags;
                
                if(viz.m_graphic.valid()){
                    poolInfo.queueFamilyIndex   = viz.m_graphic.family;
                    if (vkCreateCommandPool(device, &poolInfo, nullptr, &graphic) != VK_SUCCESS) 
                        throw VqException("Failed to create a graphic command pool!");
                }
                if(viz.m_compute.valid()){
                    poolInfo.queueFamilyIndex   = viz.m_compute.family;
                    if (vkCreateCommandPool(device, &poolInfo, nullptr, &compute) != VK_SUCCESS) 
                        throw VqException("Failed to create a compute command pool!");
                }
            }
            catch(VqException& ex)
            {
                dtor();
                throw;
            }
        }
        
        ViThread::~ViThread()
        {
            dtor();
        }
        
        void    ViThread::dtor()
        {
            if(device){
                if(descriptors){
                    vkDestroyDescriptorPool(device, descriptors, nullptr);
                    descriptors = nullptr;
                }
                if(graphic){
                    vkDestroyCommandPool(device, graphic, nullptr);
                    graphic = nullptr;
                }
                if(compute){
                    vkDestroyCommandPool(device, compute, nullptr);
                    compute = nullptr;
                }
                device  = nullptr;
            }
        }

        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        
        void         Visualizer::callback_resize(GLFWwindow* win, int, int)
        {
            Visualizer    *v  = (Visualizer*) glfwGetWindowUserPointer(win);
            if(v){
                v -> m_rebuildSwap    = true;
                assert(v->m_viewer);
                if(v->m_viewer) [[likely]]
                    v->m_viewer->window_resized();
            }
        }
        
        //  ----------------------------------------------------------------------------

        Visualizer::Visualizer(const ViewerCreateInfo& vci, Viewer *w) 
        {
            m_viewer        = w;
            
            try {
                _ctor(vci);
            }
            catch(VqException& ex)
            {
                _dtor();
                throw;
            }
        }

        Visualizer::~Visualizer()
        {
            _dtor();
        }

        namespace {
            std::vector<float>      make_weights(const VqQueueSpec& qs, uint32_t mincnt=0)
            { 
                if(const std::vector<float>*p = std::get_if<std::vector<float>>(&qs)){
                    if((!p->empty()) && (p->size() >= (size_t) mincnt))
                        return *p;
                }
                uint32_t    cnt = mincnt;
                if(const uint32_t* p = std::get_if<uint32_t>(&qs))
                    cnt = std::max(cnt, *p);
                if(const bool* p = std::get_if<bool>(&qs)){
                    if(*p)
                        cnt = std::max<uint32_t>(cnt, 1);
                }
                if(cnt){
                    std::vector<float> ret;
                    ret.resize(cnt, 1.);
                    return ret;
                }
                return std::vector<float>();
            }
        }


        void Visualizer::_ctor(const ViewerCreateInfo& vci)
        {
            Application*   app    = Application::app();
            if(!app)
                throw VqException("Unintialized or no application present!");

            app -> init_vulkan();

            m_instance    = app->instance();
            if(!m_instance)
                throw VqException("Vulkan has not been initialized!");



            //  ================================
            //  SELECT GPU (ie, physical device)

            m_physical                    = vci.device;
            if(!m_physical){
                m_physical  = vqFirstDevice();
                if(!m_physical)
                    throw VqException("Cannot create m_window without any devices!");
            }
            
            vkGetPhysicalDeviceProperties(m_physical, &m_deviceInfo);
            vkGetPhysicalDeviceMemoryProperties(m_physical, &m_memoryInfo);
            

            //  ================================
            //  CREATE GLFW WINDOW

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_FLOATING, vci.floating ? GLFW_TRUE : GLFW_FALSE);
            glfwWindowHint(GLFW_DECORATED, vci.decorated ? GLFW_TRUE : GLFW_FALSE);
            glfwWindowHint(GLFW_RESIZABLE, vci.resizable ? GLFW_TRUE : GLFW_FALSE);
            
            int wx  = std::max(1,vci.size.width());
            int wy  = std::max(1,vci.size.height());

            m_window = glfwCreateWindow(wx, wy, vci.title, vci.monitor.monitor(), nullptr);
            if(!m_window)
                throw VqException("Unable to create GLFW m_window");
            
            //  Register pointer & callbacks
            glfwSetWindowUserPointer(m_window, this);
            glfwSetWindowSizeCallback(m_window, callback_resize);

            //  ================================
            //  GLFW "SURFACE"

            if(glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface) != VK_SUCCESS)
                throw VqException("Unable to create window surface!");
                
            for(auto pm : vqGetPhysicalDeviceSurfacePresentModesKHR(m_physical, m_surface))
                m_presentModes.insert((PresentMode::enum_t) pm);
            m_surfaceFormats        = vqGetPhysicalDeviceSurfaceFormatsKHR(m_physical, m_surface);
            
            // right now, cheating on format & color space
            m_surfaceFormat         = VK_FORMAT_B8G8R8A8_SRGB;
            m_surfaceColorSpace     = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

            //  ================================
            //  LOGICAL DEVICE CREATION
            //
            //  Buckle up, this is a long one

                // list extensions, augmenting with swap chain
                
            std::vector<const char*>    devExtensions = vci.extensions;
            devExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

                //  Query the GPU for valid queue families
            auto queueInfos         = vqFindQueueFamilies(m_physical, m_surface);
            
                //  And we need to create them... so request
            std::vector<VkDeviceQueueCreateInfo> qci;
            
                //  graphic is required....
            std::vector<float>  graphicWeights  = make_weights(vci.graphic, 1);
            if(!queueInfos.graphics.has_value()){
                throw VqException("No graphic queue capability!");
            } else {
                m_graphic.family        = queueInfos.graphics.value();
                VqDeviceQueueCreateInfo info;
                info.queueFamilyIndex   = m_graphic.family;
                info.queueCount         = (uint32_t) graphicWeights.size();
                info.pQueuePriorities   = graphicWeights.data();
                qci.push_back(info);
            }
            
                //  present is required....
            std::vector<float>  presentWeights  = make_weights(vci.present, 1);
            if(!queueInfos.present.has_value()){
                throw VqException("No graphic queue capability!");
            } else {
                m_present.family        = queueInfos.present.value();
                VqDeviceQueueCreateInfo info;
                info.queueFamilyIndex   = m_present.family;
                info.queueCount         = (uint32_t) presentWeights.size();
                info.pQueuePriorities   = presentWeights.data();
                qci.push_back(info);
            }
            
                //  Determine if compute is requested, create the request
            std::vector<float>  computeWeights  = make_weights(vci.compute, 0);
            if(!computeWeights.empty()){
                if(!queueInfos.compute.has_value()){
                    throw VqException("No compute queue capability!");
                } else {
                    m_compute.family        = queueInfos.compute.value();
                    VqDeviceQueueCreateInfo info;
                    info.queueFamilyIndex   = m_compute.family;
                    info.queueCount         = (uint32_t) computeWeights.size();
                    info.pQueuePriorities   = computeWeights.data();
                    qci.push_back(info);
                }
            }

                //  Determine if video decoding is requested, create the request
            std::vector<float>  videoDecWeights = make_weights(vci.video_decode, 0);
            if(!videoDecWeights.empty()){
                if(!queueInfos.videoDecode.has_value()){
                    throw VqException("No video decode queue capability!");
                } else {
                    m_videoDecode.family    = queueInfos.videoDecode.value();
                    VqDeviceQueueCreateInfo info;
                    info.queueFamilyIndex   = m_videoDecode.family;
                    info.queueCount         = (uint32_t) videoDecWeights.size();
                    info.pQueuePriorities   = videoDecWeights.data();
                    qci.push_back(info);
                }
            }
            
                //  Determine if video encoding is requested, create the request
            std::vector<float>  videoEncWeights = make_weights(vci.video_encode, 0);
            if(!videoEncWeights.empty()){
                if(!queueInfos.videoEncode.has_value()){
                    throw VqException("No video encode queue capability!");
                } else {
                    m_videoEncode.family    = queueInfos.videoEncode.value();
                    VqDeviceQueueCreateInfo info;
                    info.queueFamilyIndex   = m_videoEncode.family;
                    info.queueCount         = (uint32_t) videoEncWeights.size();
                    info.pQueuePriorities   = videoEncWeights.data();
                    qci.push_back(info);
                }
            }
            
            std::sort(qci.begin(), qci.end(), [](const VkDeviceQueueCreateInfo& a, const VkDeviceQueueCreateInfo& b) -> bool {
                return a.queueFamilyIndex < b.queueFamilyIndex;
            });
            
            for(size_t i=0;i<qci.size()-1;++i){
                VkDeviceQueueCreateInfo&    prev  = qci[i];
                VkDeviceQueueCreateInfo&    next  = qci[i+1];
                if(prev.queueFamilyIndex != next.queueFamilyIndex)
                    continue;
                
                //  carry the request forward
                if(next.queueCount < prev.queueCount){
                    //  steal the pointer
                    next.queueCount         = prev.queueCount;
                    next.pQueuePriorities   = prev.pQueuePriorities;
                }
                prev.queueFamilyIndex   = UINT32_MAX;
            }
            
            auto qitr = std::remove_if(qci.begin(), qci.end(), [](const VkDeviceQueueCreateInfo& a) -> bool {
                return a.queueFamilyIndex == UINT32_MAX;
            });
            if(qitr != qci.end())
                qci.erase(qitr, qci.end());
                    
            //  And with that, we have the queues all lined up, ready to be created.

            VkPhysicalDeviceFeatures    gpu_features{};
            if(vci.fill_non_solid)
                gpu_features.fillModeNonSolid    = VK_TRUE;

            VqDeviceCreateInfo          deviceCreateInfo;
            deviceCreateInfo.pQueueCreateInfos        = qci.data();
            deviceCreateInfo.queueCreateInfoCount     = (uint32_t) qci.size();
            deviceCreateInfo.pEnabledFeatures         = &gpu_features;
            
            const auto& layers = app->layers();
            
            deviceCreateInfo.enabledLayerCount          = (uint32_t) layers.size();
            if(deviceCreateInfo.enabledLayerCount)
                deviceCreateInfo.ppEnabledLayerNames    = layers.data();
        
            deviceCreateInfo.enabledExtensionCount      = (uint32_t) devExtensions.size();
            deviceCreateInfo.ppEnabledExtensionNames    = devExtensions.data();
            
            if(vkCreateDevice(m_physical, &deviceCreateInfo, nullptr, &m_device) != VK_SUCCESS)
                throw VqException("Unable to create logical device!");
            
            //  ================================
            //  GETTING THE QUEUES

            m_graphic.set(m_device, graphicWeights.size());
            m_present.set(m_device, presentWeights.size());
            if(m_compute.family != UINT32_MAX)
                m_compute.set(m_device, computeWeights.size());
            if(m_videoEncode.family != UINT32_MAX)
                m_videoEncode.set(m_device, videoEncWeights.size());
            if(m_videoDecode.family != UINT32_MAX)
                m_videoDecode.set(m_device, videoDecWeights.size());

            //  ================================
            //  ALLOCATOR

            VmaAllocatorCreateInfo      allocatorCreateInfo;
            allocatorCreateInfo.instance                        = m_instance;
            allocatorCreateInfo.physicalDevice                  = m_physical;
            allocatorCreateInfo.device                          = m_device;
            allocatorCreateInfo.vulkanApiVersion                = app->app_info().vulkan_api;
            allocatorCreateInfo.preferredLargeHeapBlockSize     = (VkDeviceSize) vci.chunk_size;
            vmaCreateAllocator(&allocatorCreateInfo, &m_allocator);

            //  ================================
            //  DESCRIPTOR & COMMAND POOL

            m_descriptorCount   = std::max(MIN_DESCRIPTOR_COUNT, vci.descriptors);
            m_thread            = std::make_unique<ViThread>(*this);
            
            //  ================================
            //  RENDER PASS

            VkAttachmentDescription colorAttachment{};
            colorAttachment.format          = m_surfaceFormat;
            colorAttachment.samples         = VK_SAMPLE_COUNT_1_BIT;
            colorAttachment.loadOp          = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachment.storeOp         = VK_ATTACHMENT_STORE_OP_STORE;        
            colorAttachment.stencilLoadOp   = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachment.stencilStoreOp  = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachment.initialLayout   = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachment.finalLayout     = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            
            VkAttachmentReference colorAttachmentRef{};
            colorAttachmentRef.attachment = 0;
            colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkSubpassDescription subpass{};
            subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;        

            subpass.colorAttachmentCount = 1;
            subpass.pColorAttachments = &colorAttachmentRef;
            
            VqRenderPassCreateInfo renderPassInfo;
            renderPassInfo.attachmentCount = 1;
            renderPassInfo.pAttachments = &colorAttachment;
            renderPassInfo.subpassCount = 1;
            renderPassInfo.pSubpasses = &subpass;

            if (vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS) 
                throw VqException("Unable to create the render pass!");

            //  ================================
            //  PRESENT MODE

            m_presentMode               = m_presentModes.contains(vci.pmode) ? vci.pmode : PresentMode{ PresentMode::Fifo };
                
                
            //  ================================
            //  OLDER STUFF

       
            imageAvailableSemaphore     = VqSemaphore(m_device);
            renderFinishedSemaphore     = VqSemaphore(m_device);
            inFlightFence               = VqFence(m_device);

            if(!init(dynamic))
                throw VqException("");
            
            builder = std::thread([this](){
                this->run();
            });
        }
        

        void Visualizer::_dtor()
        {
            terminating = true;
            builder.join();
            if(m_device){
                vkDeviceWaitIdle(m_device);
            }
            
            for(auto& i : objects){
                if(i.second)
                    delete i.second;
            }
            objects.clear();

            for(auto& i : pipelines){
                if(i.second)
                    delete i.second;
            }
            pipelines.clear();
            
            kill(dynamic);
            
            imageAvailableSemaphore     = {};
            renderFinishedSemaphore     = {};
            inFlightFence               = {};
            
            if(m_renderPass){
                vkDestroyRenderPass(m_device, m_renderPass, nullptr);
                m_renderPass = nullptr;
            }
            
            m_thread                    = {};
            
            if(m_allocator){
                vmaDestroyAllocator(m_allocator);
                m_allocator = nullptr;
            }
            
            if(m_device){
                vkDestroyDevice(m_device, nullptr);
                m_device                = nullptr;
            }
            m_graphic                   = {};
            m_present                   = {};
            m_compute                   = {};
            m_videoEncode               = {};
            m_videoDecode               = {};
            
            if(m_surface){
                vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
                m_surface               = nullptr;
            }
            m_physical                  = nullptr;
            if(m_window){
                glfwDestroyWindow(m_window);
                m_window                = nullptr;
            }
            m_physical                  = nullptr;
        }


        bool Visualizer::init(VqDynamic&ds, VkSwapchainKHR old)
        {
            VqSwapchain::Config scfg;
            scfg.pmode          = (VkPresentModeKHR) m_presentMode.value();
            scfg.old            = old;
            ds.swapchain        = VqSwapchain(*this, scfg);
            
            ds.images           = ds.swapchain.images();
            ds.imageCount       = ds.images.size();
            ds.imageViews       = VqImageViews(*this, ds.images);
            ds.frameBuffers     = VqFrameBuffers(m_device, m_renderPass, ds.swapchain.extents(), ds.imageViews);
            ds.commandBuffers   = VqCommandBuffers(m_device, m_thread->graphic, 1);

            return true;            
        }



        void Visualizer::kill(VqDynamic&ds)
        {
            ds.commandBuffers   = {};
            ds.frameBuffers     = {};
            ds.imageViews       = {};
            ds.swapchain        = {};
        }

        void    Visualizer::run()
        {
            using namespace std::chrono_literals;
            while(!terminating){
                std::this_thread::sleep_for(1ms);
            }
        }


        std::pair<ViPipeline*,bool>    Visualizer::pipeline(uint64_t i)
        {
            auto j = pipelines.find(i);
            if(j!=pipelines.end())
                return {j->second,false};
            ViPipeline* p   = new ViPipeline;
            pipelines[i]    = p;
            return {p, true};
        }
        
        std::pair<ViObject*,bool>    Visualizer::object(uint64_t i)
        {
            auto j = objects.find(i);
            if(j!=objects.end())
                return {j->second,false};
            ViObject* p   = new ViObject;
            objects[i]    = p;
            return {p, true};
        }

        //////////////////////////////////////


        VkSurfaceCapabilitiesKHR    Visualizer::surface_capabilities() const
        {
            VkSurfaceCapabilitiesKHR    ret;
            if(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physical, m_surface, &ret) != VK_SUCCESS)
                throw VqException("Unable to get surface capabilities");
            return ret;
        }


        VkColorSpaceKHR Visualizer::surface_color_space(VkFormat fmt) const
        {
            for(auto& f : m_surfaceFormats)
                if(fmt == f.format)
                    return f.colorSpace;
            return VK_COLOR_SPACE_MAX_ENUM_KHR;
        }
        
        YQ_INVOKE(
            writer<Viewer>();
        )
        
    }
}

