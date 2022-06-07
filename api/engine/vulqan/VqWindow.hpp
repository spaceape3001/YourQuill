////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "VqFence.hpp"
#include <basic/Object.hpp>
#include <basic/Ref.hpp>
#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>
#include <math/preamble.hpp>
#include <math/ColorRgb.hpp>
#include <math/shape/Size2.hpp>
#include <math/vec/Vector2.hpp>
#include <vulkan/vulkan_core.h>
#include <engine/vulqan/VqMonitor.hpp>
#include <optional>

struct HelloApp;
struct GLFWwindow;

namespace yq {
    namespace engine {
        struct PipelineConfig;
        
        struct WindowCreateInfo {
            VkPhysicalDevice    device   = nullptr;
        
            const char*                 title    = "(untitled)";
            Size2I                      size     = { 1920, 1080 };
            std::optional<Vector2I>     position;
                //!  Set to get full screen, windowed otherwise
            VqMonitor                   monitor;
            VkPresentModeKHR            pmode   = VK_PRESENT_MODE_FIFO_KHR;
            
            
                //!  This is the background color
            ColorRgbF               clear   = { 0., 0., 0., 1. };
            
                //!  Set to make always-on-top
            bool                    floating    = false;
            
                //!  Set to make decorated
            bool                    decorated   = true;
            
                //!  Set to make user-resizable
            bool                    resizable   = false;
            
                //!     Set to get descriptors allocated
            uint32_t                descriptors = 0;
            
            WindowCreateInfo(){}
        };
        
        struct VqWindowInfo : public ObjectInfo {
            VqWindowInfo(std::string_view, ObjectInfo&, const std::source_location& sl = std::source_location::current());
        };
        
        
        /*! \brief A basic window
        
            This encapsulates the GLFW & Vulkan into a single window, requires
            a Vulkan instance.
        */
        class VqWindow : public Object, public RefCount, trait::not_copyable, trait::not_moveable {
            YQ_OBJECT_INFO(VqWindowInfo)
            YQ_OBJECT_DECLARE(VqWindow, Object)
        public:
        
            static constexpr const uint32_t kMinimumDescriptors = 1000;
            
                //! Polls for events (does not loiter)
            static void poll_events();


                // Creates a window bound to the specified vulkan instance
            VqWindow(const WindowCreateInfo& i = WindowCreateInfo());
            
            ~VqWindow();
            

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
            
            VkQueue             graphics_queue() const { return m_graphics.queue; }
            
            uint32_t            graphics_queue_family() const { return m_graphics.family; }

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
            void                set_clear(const ColorRgbF&);

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
            
            
                //! Width of the window
            int                 width() const;

                //! GLFW window handle
            GLFWwindow*         window() const { return m_window; }
            
            //  This is the "DRAW" pass, do it all, whatever the result is
            virtual bool        draw();

        protected:
            virtual void        draw_vulqan(VkCommandBuffer){}
            struct Pipeline;
        
        private:
            
            struct Command;

            bool    init(const WindowCreateInfo& i);
            bool    init_physical(const WindowCreateInfo& i);
            bool    init_window(const WindowCreateInfo& i);
            bool    init_surface();
            bool    init_logical();
            bool    init_command_pool();
            bool    init_render_pass();
            bool    init_descriptor_pool(const WindowCreateInfo&i);
            bool    init_sync();
            void    kill();
            bool    record(VkCommandBuffer, uint32_t);
            
            struct Queue {
                VkQueue             queue   = nullptr;
                uint32_t            family  = UINT32_MAX;
                VkPresentModeKHR    mode    = {};
                
                void    kill(VqWindow*);
            };
            
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

            
            VkPhysicalDevice            m_physical                  = nullptr;
            GLFWwindow*                 m_window                    = nullptr;
            VkSurfaceKHR                m_surface                   = nullptr;
            Queue                       m_graphics, m_present;
            
            VkQueue                     m_graphicsQueue             = nullptr;
            uint32_t                    m_graphicsQueueFamily       = UINT32_MAX;
            VkQueue                     m_presentQueue              = nullptr;
            VkDevice                    m_device                    = nullptr;
            VkPresentModeKHR            m_presentMode;
            VkFormat                    m_surfaceFormat;
            VkColorSpaceKHR             m_surfaceColorSpace;
            VkClearValue                m_clear;
            VkCommandPool               m_commandPool               = nullptr;
            VkRenderPass                m_renderPass                = nullptr;
            VkSemaphore                 m_imageAvailableSemaphore   = nullptr;
            VkSemaphore                 m_renderFinishedSemaphore   = nullptr;
            VqFence                     m_inFlightFence;
            VkDescriptorPool            m_descriptorPool            = nullptr;
            uint32_t                    m_descriptorCount           = 0;
            std::atomic<bool>           m_rebuildSwap               = { false };

            
            DynamicStuff            m_dynamic;
            bool                    init(DynamicStuff&, VkSwapchainKHR old=nullptr);
            void                    kill(DynamicStuff&);
            
            
            //VkPipeline                  m_lastPipeline  = nullptr;
            void        rebuild_onward();
            virtual void        window_resized(){}
            virtual void        viewport_changed(){}
            
            static void callback_resize(GLFWwindow*, int, int);
        };
        
        struct VqWindow::Pipeline {
            VkPipelineLayout    layout      = nullptr;
            VkPipeline          pipeline    = nullptr;
            
            bool    init(VqWindow*, const PipelineConfig&);
            void    kill(VqWindow*);
        };

    }
}
