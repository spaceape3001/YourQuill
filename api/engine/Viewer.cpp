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
            renderPassInfo.renderPass = m->renderPass;
            renderPassInfo.framebuffer = m->dynamic.frameBuffers[imageIndex];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = m->dynamic.swapchain.extents();

            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &m->clear;
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
            
            bool    rebuild = m->rebuildSwap.exchange(false);
            if(rebuild){
                vkDeviceWaitIdle(m->device);
                VqDynamic        newStuff;
                m->init(newStuff, m->dynamic.swapchain);
                std::swap(m->dynamic, newStuff);
                m->kill(newStuff);
                
                //  resize notifications...
            }
            
            
            VkCommandBuffer cmdbuf = m->dynamic.commandBuffers[0];
            m->inFlightFence.wait_reset();

            uint32_t imageIndex = 0;
            vkAcquireNextImageKHR(m->device, m->dynamic.swapchain, UINT64_MAX, m->imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
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

            if (vkQueueSubmit(m->device.graphics(0), 1, &submitInfo, m->inFlightFence) != VK_SUCCESS) {
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
            vkQueuePresentKHR(m->device.present(0), &presentInfo);
            
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
            if(m->window)
                glfwRequestWindowAttention(m->window);
        }

        void Viewer::close()
        {
            if(m->window)
                glfwSetWindowShouldClose(m->window, GLFW_TRUE);
        }

        VkColorSpaceKHR     Viewer::color_space() const 
        { 
            return m->surface.color_space(); 
        }

        VkCommandBuffer     Viewer::command_buffer() const
        {
            return m->dynamic.commandBuffers[0];
        }

        VkCommandPool       Viewer::command_pool() const 
        { 
            return m->commandPool; 
        }
        
        VkDescriptorPool    Viewer::descriptor_pool() const 
        { 
            return m->descriptorPool; 
        }
        
        VkDevice            Viewer::device() const 
        { 
            return m->device; 
        }

        void Viewer::focus()
        {
            if(m->window)
                glfwFocusWindow(m->window);
        }

        VkFormat    Viewer::format() const 
        { 
            return m->surface.format(); 
        }

        VkQueue     Viewer::graphics_queue() const 
        { 
            return m->device.graphics(0); 
        }
        
        uint32_t    Viewer::graphics_queue_family() const 
        { 
            return m->device.graphics().family(); 
        }

        int  Viewer::height() const
        {
            if(!m->window)
                return 0;
        
            int ret;
            glfwGetWindowSize(m->window, nullptr, &ret);
            return ret;
        }

        void Viewer::hide()
        {
            if(m->window)
                glfwHideWindow(m->window);
        }

        void Viewer::iconify()
        {
            if(m->window)
                glfwIconifyWindow(m->window);
        }

        bool        Viewer::is_decorated() const
        {
            if(!m->window)
                return false;
            return glfwGetWindowAttrib(m->window, GLFW_DECORATED) != 0;
        }
        
        bool        Viewer::is_focused() const
        {
            if(!m->window)
                return false;
            return glfwGetWindowAttrib(m->window, GLFW_FOCUSED ) != 0;
        }
        
        bool        Viewer::is_floating() const
        {
            if(!m->window)
                return false;
            return glfwGetWindowAttrib(m->window, GLFW_FLOATING) != 0;
        }
        
        bool        Viewer::is_fullscreen() const
        {
            if(!m->window)
                return false;
            return glfwGetWindowMonitor(m->window) != nullptr;
        }
        
        bool        Viewer::is_hovered() const
        {
            if(!m->window)
                return false;
            return glfwGetWindowAttrib(m->window, GLFW_HOVERED) != 0;
        }
        
        bool        Viewer::is_iconified() const
        {
            if(!m->window)
                return false;
            return glfwGetWindowAttrib(m->window, GLFW_ICONIFIED) != 0;
        }

        bool        Viewer::is_maximized() const
        {
            if(!m->window)
                return false;
            return glfwGetWindowAttrib(m->window, GLFW_MAXIMIZED) != 0;
        }
        
        bool        Viewer::is_resizable() const
        {
            if(!m->window)
                return false;
            return glfwGetWindowAttrib(m->window, GLFW_RESIZABLE) != 0;
        }
        
        bool        Viewer::is_visible() const
        {
            if(!m->window)
                return false;
            return glfwGetWindowAttrib(m->window, GLFW_VISIBLE) != 0;
        }
        
        VkDevice    Viewer::logical() const 
        { 
            return m->device; 
        }

        void        Viewer::maximize()
        {
            if(m->window)
                glfwMaximizeWindow(m->window);
        }

        VqMonitor   Viewer::monitor() const
        {
            if(m->window)
                return VqMonitor(glfwGetWindowMonitor(m->window));
            return VqMonitor();
        }

        VkPhysicalDevice    Viewer::physical() const 
        { 
            return m->physical; 
        }

        Vector2I    Viewer::position() const
        {
            if(!m->window)
                return {};
            Vector2I   ret;
            glfwGetWindowPos(m->window, &ret.x, &ret.y);
            return ret;
        }



        VkRenderPass Viewer::render_pass() const
        {
            return m->renderPass;
        }

        void        Viewer::restore()
        {
            if(m->window)
                glfwRestoreWindow(m->window);
        }

        void        Viewer::set_clear(const RGBA4F&i)
        {
            m->set_clear(i);
        }

        void        Viewer::set_position(const Vector2I& pos)
        {
            set_position(pos.x, pos.y);
        }
        
        void        Viewer::set_position(int x, int y)
        {
            if(m->window){
                glfwSetWindowPos(m->window, x, y);
            }
        }

        void        Viewer::set_size(const Size2I& sz)
        {
            set_size(sz.x, sz.y);
        }

        void        Viewer::set_size(int w, int h)
        {
            if(m->window){
                glfwSetWindowSize(m->window, std::max(1, w), std::max(1, h));
            }
        }

        void        Viewer::set_title(const char*z)
        {
            if(m->window && z){
                glfwSetWindowTitle(m->window, z);
            }
        }

        void        Viewer::set_title(const std::string&z)
        {
            set_title(z.c_str());
        }

        bool        Viewer::should_close() const
        {
            if(!m->window) 
                return true;
            return glfwWindowShouldClose(m->window);
        }

        void        Viewer::show()
        {
            if(m->window)
                glfwShowWindow(m->window);
        }

        Size2I      Viewer::size() const
        {
            if(!m->window)
                return {};
            Size2I  ret;
            glfwGetWindowSize(m->window, &ret.x, &ret.y);
            return ret;
        }

        VkSurfaceKHR        Viewer::surface() const 
        { 
            return m->surface; 
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
        
        int  Viewer::width() const
        {
            if(!m->window)
                return 0;
        
            int ret;
            glfwGetWindowSize(m->window, &ret, nullptr);
            return ret;
        }
        
        GLFWwindow*         Viewer::window() const 
        { 
            return m->window; 
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
        ////////////////////////////////////////////////////////////////////////////////

        Visualizer::Visualizer(const ViewerCreateInfo& i, Viewer *w) 
        {
            user        = w;
            
            instance    = Application::vulkan();
            if(!instance)
                throw VqException("Vulkan has not been initialized!");

            VkPhysicalDevice    pdev    = i.device;
            pdev  = i.device;
            if(!pdev){
                pdev  = vqFirstDevice();
                if(!pdev)
                    throw VqException("Cannot create window without any devices!");
            }
            physical                    = VqGPU(pdev);
            yNotice() << "Using (" << to_string(physical.device_type()) << "): " << physical.device_name();
            window                      = VqWindow(*this, i);

            surface                     = VqSurface(physical, window);
            
            VqDevice::Config    dci;
            dci.extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
            device                      = VqDevice(surface, dci);
            allocator                   = VqAllocator(*this);
       
            VkPresentModeKHR pmdef      = (VkPresentModeKHR) i.pmode.value();
       
            presentMode                 = surface.supports(pmdef) ? pmdef : VK_PRESENT_MODE_FIFO_KHR;
                
            commandPool                 = VqCommandPool(device, device.graphics().family());
            renderPass                  = VqRenderPass(*this);
            imageAvailableSemaphore     = VqSemaphore(device);
            renderFinishedSemaphore     = VqSemaphore(device);
            inFlightFence               = VqFence(device);
            descriptorPool              = VqDescriptorPool(device, i.descriptors);

            if(!init(dynamic))
                throw VqException("");
            
            set_clear(i.clear);
            
            builder = std::thread([this](){
                this->run();
            });
        }

        bool Visualizer::init(VqDynamic&ds, VkSwapchainKHR old)
        {
            VqSwapchain::Config scfg;
            scfg.pmode          = presentMode;
            scfg.old            = old;
            ds.swapchain        = VqSwapchain(device, surface, scfg);
            
            ds.images           = ds.swapchain.images();
            ds.imageCount       = ds.images.size();
            ds.imageViews       = VqImageViews(device, surface, ds.images);
            ds.frameBuffers     = VqFrameBuffers(device, renderPass, ds.swapchain.extents(), ds.imageViews);
            ds.commandBuffers   = VqCommandBuffers(commandPool, 1);

            return true;            
        }

        Visualizer::~Visualizer()
        {
            terminating = true;
            builder.join();
            if(device){
                vkDeviceWaitIdle(device);
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
            
            descriptorPool              = {};
            imageAvailableSemaphore     = {};
            renderFinishedSemaphore     = {};
            inFlightFence               = {};
            renderPass                  = {};
            commandPool                 = {};
            
            allocator                   = {};
            device                      = {};
            
            surface                     = {};
            window                      = {};
            physical                    = {};
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

        void    Visualizer::set_clear(const RGBA4F&i)
        {
            clear = VkClearValue{{{ i.red, i.green, i.blue, i.alpha }}};
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
        
        YQ_INVOKE(
            writer<Viewer>();
        )
        
    }
}

