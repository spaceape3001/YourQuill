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
#include <math/vec/Vec2.hpp>
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
        
            const char*             title    = "(untitled)";
            Size2I                  size     = { 1920, 1080 };
            std::optional<Vec2I>    position;
                //!  Set to get full screen, windowed otherwise
            VqMonitor               monitor;
            VkPresentModeKHR        pmode   = VK_PRESENT_MODE_FIFO_KHR;
            
                //!  This is the background color
            ColorRgbF               clear   = { 0., 0., 0., 1. };
            
                //!  Set to make always-on-top
            bool                    floating    = false;
            
                //!  Set to make decorated
            bool                    decorated   = true;
            
                //!  Set to make user-resizable
            bool                    resizable   = false;
            
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
            
                //! Polls for events (does not loiter)
            static void poll_events();


                // Creates a window bound to the specified vulkan instance
            VqWindow(const WindowCreateInfo& i = WindowCreateInfo());
            
            ~VqWindow();
            

                //! Calls user's attention to window
            void                attention();

                //! Closes (politely) this window....
            void                close();
            
            VkColorSpaceKHR     color_space() const { return m_surfaceFormat.colorSpace; }
            
            VkCommandBuffer     command_buffer() const;
            
            VkCommandPool       command_pool() const;
            
            VkDevice            device() const { return m_device; }

                //! Brings window to front & input focus
            void                focus();
            
            VkFormat            format() const { return m_surfaceFormat.format; }

                //! Good & initialized window
            bool                good() const { return m_window != nullptr; }
            
            VkQueue             graphics_queue() const { return m_graphicsQueue; }
            
            uint32_t            graphics_queue_family() const { return m_graphicsQueueFamily; }

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
            Vec2I               position() const;
            
            VkRenderPass        render_pass() const;
            
                //! Restores the window to non-fullscreen/iconify
            void                restore();

                //! Sets the background color
            void                set_clear(const ColorRgbF&);

                //! Sets the window position
            void                set_position(const Vec2I&);

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
            void    kill();
            bool    record(VkCommandBuffer, uint32_t);
            
            VkPhysicalDevice            m_physical              = nullptr;
            GLFWwindow*                 m_window                = nullptr;
            VkSurfaceKHR                m_surface               = nullptr;
            VkQueue                     m_graphicsQueue         = nullptr;
            uint32_t                    m_graphicsQueueFamily   = UINT32_MAX;
            VkQueue                     m_presentQueue          = nullptr;
            VkDevice                    m_device                = nullptr;
            VkPresentModeKHR            m_presentMode;
            VkSurfaceFormatKHR          m_surfaceFormat;
            VkClearValue                m_clear;
            
            struct SwapChain {
                VkSwapchainKHR              handle  = nullptr;
                VkExtent2D                  extents  = { 0, 0 };
                std::vector<VkImage>        images;
                std::vector<VkImageView>    imageViews;
                uint32_t                    imageCount      = 0;
                uint32_t                    minImageCount   = 0;
                
                bool        init(VqWindow*);
                void        kill(VqWindow*);
                VkRect2D    def_scissor() const;
                VkViewport  def_viewport() const;
            };
            
            SwapChain                   m_swapChain;
            
            struct RenderPass {
                VkRenderPass            handle  = nullptr;

                bool                    init(VqWindow*);
                void                    kill(VqWindow*);
            };
            
            RenderPass                  m_renderPass;
            
            struct FrameBuffers {
                std::vector<VkFramebuffer> buffers;
                
                bool                    init(VqWindow*);
                void                    kill(VqWindow*);
            };
            
            FrameBuffers                m_frameBuffers;
            
            struct Command {
                VkCommandPool           pool;
                VkCommandBuffer         buffer;

                bool                    init(VqWindow*);
                void                    kill(VqWindow*);
            };
            
            Command                     m_command;
            
            struct Sync {
                VkSemaphore         imageAvailable;
                VkSemaphore         renderFinished;
                VqFence             inFlightFence;

                bool                    init(VqWindow*);
                void                    kill(VqWindow*);
            };
            
            Sync                        m_sync;
            
            //VkPipeline                  m_lastPipeline  = nullptr;
        };
        
        struct VqWindow::Pipeline {
            VkPipelineLayout    layout      = nullptr;
            VkPipeline          pipeline    = nullptr;
            
            bool    init(VqWindow*, const PipelineConfig&);
            void    kill(VqWindow*);
        };

    }
}
