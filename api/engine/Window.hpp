////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "WindowCreateInfo.hpp"

#include <basic/Object.hpp>
#include <basic/Ref.hpp>
#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>
#include <math/preamble.hpp>
#include <math/ColorRgba.hpp>
#include <math/shape/Size2.hpp>
#include <math/vec/Vector2.hpp>
#include <engine/vulqan/VqCommandPool.hpp>
#include <engine/vulqan/VqFence.hpp>
#include <engine/vulqan/VqGPU.hpp>
#include <engine/vulqan/VqMonitor.hpp>
#include <engine/vulqan/VqSemaphore.hpp>
#include <engine/vulqan/VqSurface.hpp>
#include <engine/vulqan/VqWindow.hpp>
#include <engine/vulqan/VqQueues.hpp>
#include <vulkan/vulkan_core.h>

#include <functional>
#include <optional>
#include <memory>

struct HelloApp;
struct GLFWwindow;

namespace yq {
    namespace engine {
        struct PipelineConfig;
        class VqWindow;
        
        
        struct WindowInfo : public ObjectInfo {
            WindowInfo(std::string_view, ObjectInfo&, const std::source_location& sl = std::source_location::current());
        };
        
        
        /*! \brief A basic window
        
            This encapsulates the GLFW & Vulkan into a single window, requires
            a Vulkan instance.
        */
        class Window : public Object, public RefCount, trait::not_copyable, trait::not_moveable {
            YQ_OBJECT_INFO(WindowInfo)
            YQ_OBJECT_DECLARE(Window, Object)
        public:
        
            static constexpr const uint32_t kMinimumDescriptors = 1000;
            
                //! Polls for events (does not loiter)
            static void poll_events();


                // Creates a window bound to the specified vulkan instance
            Window(const WindowCreateInfo& i = WindowCreateInfo());
            
            ~Window();
            

                //! Calls user's attention to window
            void                attention();

                //! Closes (politely) this window....
            void                close();
            
            VkColorSpaceKHR     color_space() const { return m_surfaceColorSpace; }
            
            VkCommandBuffer     command_buffer() const;
            
            VkCommandPool       command_pool() const { return m_commandPool; }
            
            VkDescriptorPool    descriptor_pool() const { return m_descriptorPool; }
            
            VkDevice            device() const { return m_device; }

                //! Brings window to front & input focus
            void                focus();
            
            VkFormat            format() const { return m_surfaceFormat; }

                //! Good & initialized window
            bool                good() const { return m_window != nullptr; }
            
            VkQueue             graphics_queue() const;
            
            uint32_t            graphics_queue_family() const;

                //! Height of the window
            int                 height() const;
            
                //! Hide the window
            void                hide();

                //! Iconifies (minimizes) window
            void                iconify();

            
                //! TRUE if the window has standard decorations (close/buttons/frame)
            bool                is_decorated() const;
                //! TRUE if the window has input focus
            bool                is_focused() const;
                //! TRUE if window is floating (ie always-on-top)
            bool                is_floating() const;
                //! TRUE if the window is fullscreen
            bool                is_fullscreen() const;
                //! TRUE if the mouse cursor is hovering directly above us
            bool                is_hovered() const;
                //! TRUE if the window is iconified
            bool                is_iconified() const;
                //! TRUE if the window is maximized
            bool                is_maximized() const;
                //! TRUE if the window is resizable
            bool                is_resizable() const;
                //! TRUE if the window is visible
            bool                is_visible() const;
            
                //! Our device (logical)
            VkDevice            logical() const { return m_device; }

            
                //! Maximizes widnow
            void                maximize();

                //! Monitor (if fullscreen)
            VqMonitor           monitor() const;

            VkPhysicalDevice    physical() const { return m_physical; }

                //! Current window position
            Vector2I            position() const;
            
            VkRenderPass        render_pass() const;
            
                //! Restores the window to non-fullscreen/iconify
            void                restore();

                //! Sets the background color
            void                set_clear(const ColorRgbaF&);

                //! Sets the window position
            void                set_position(const Vector2I&);

                //! Sets the window position
            void                set_position(int x, int y);

                //! Sets window size
            void                set_size(const Size2I&);

                //! Sets window size
            void                set_size(int w, int h);
            
                //! Sets the window title
            void                set_title(const char*);

                //! Sets the window title
            void                set_title(const std::string&);

                //! TRUE if user wants window to close
            bool                should_close() const;
            
                //! Show the window
            void                show();

                //! Window size
            Size2I              size() const;
            
            //const VkExtent2D&   swap_extent() const { return m_swapExtent; }
            
                //! The Vulkan surface
            VkSurfaceKHR        surface() const { return m_surface; }
            
            VkRect2D            swap_def_scissor() const;
            
            VkViewport          swap_def_viewport() const;

            uint32_t            swap_image_count() const;
            uint32_t            swap_min_image_count() const;

            uint32_t            swap_width() const;
            
            uint32_t            swap_height() const;
            
            bool                valid() const { return window() != nullptr; }
            
            
                //! Width of the window
            int                 width() const;

                //! GLFW window handle
            GLFWwindow*         window() const { return m_window; }
            
            //  This is the "DRAW" pass, do it all, whatever the result is
            virtual bool        draw();

        protected:
        
            friend class VqWindow;
        
            struct Command;
            
            //struct Queue {
                //VkQueue             queue   = nullptr;
                //uint32_t            family  = UINT32_MAX;
////                VkPresentModeKHR    mode    = {};
                
                //void    kill(Window*);
            //};
            
            //! This is what needs to change with every resize!
            struct DynamicStuff {
                std::vector<VkImage>        images;
                std::vector<VkImageView>    imageViews;
                std::vector<VkFramebuffer>  frameBuffers;
                VkSwapchainKHR              swapChain           = nullptr;
                VkCommandBuffer             commandBuffer       = nullptr;
                VkExtent2D                  extents             = { 0, 0 };
                uint32_t                    imageCount          = 0;
                uint32_t                    minImageCount       = 0;

                VkRect2D    def_scissor() const;
                VkViewport  def_viewport() const;
            };

            
            VqGPU               m_physical;
            VqWindow            m_window;
            VqSurface           m_surface;
            VqQueues            m_graphics, m_present;
            VqCommandPool       m_commandPool;
            
            VkDevice            m_device                    = nullptr;
            VkPresentModeKHR    m_presentMode               = {};
            VkFormat            m_surfaceFormat;
            VkColorSpaceKHR     m_surfaceColorSpace;
            VkClearValue        m_clear;
            VkRenderPass        m_renderPass                = nullptr;
            VqSemaphore         m_imageAvailableSemaphore;
            VqSemaphore         m_renderFinishedSemaphore;
            VqFence             m_inFlightFence;
            VkDescriptorPool    m_descriptorPool            = nullptr;
            uint32_t            m_descriptorCount           = 0;
            std::atomic<bool>   m_rebuildSwap               = { false };

            
            DynamicStuff        m_dynamic;
            
            //VkPipeline                  m_lastPipeline  = nullptr;
            virtual void        window_resized(){}
            virtual void        viewport_changed(){}
            virtual void        draw_vulqan(VkCommandBuffer){}
            
        private:
            bool    init(DynamicStuff&, VkSwapchainKHR old=nullptr);
            void    kill(DynamicStuff&);
            bool    init(const WindowCreateInfo& i);
            bool    init_physical(const WindowCreateInfo& i);
            bool    init_logical();
            bool    init_render_pass();
            bool    init_descriptor_pool(const WindowCreateInfo&i);
            void    kill();
            bool    record(VkCommandBuffer, uint32_t);
            
            static void callback_resize(GLFWwindow*, int, int);
        };
        

    }
}
