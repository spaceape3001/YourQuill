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
#include <math/Size2.hpp>
#include <math/Vector2.hpp>
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
        struct VqVisualizer;
        class VqAllocator;
        struct Scene;
        struct Perspective;
        
        
        struct WindowInfo : public ObjectInfo {
            WindowInfo(std::string_view, ObjectInfo&, const std::source_location& sl = std::source_location::current());
        };
        
        
        /*! \brief A basic window
        
            This encapsulates the GLFW & Vulkan into a single window, requires
            a Vulkan instance.
        */
        class Vulqan : public Object, public RefCount, trait::not_copyable, trait::not_moveable {
            YQ_OBJECT_INFO(WindowInfo)
            YQ_OBJECT_DECLARE(Vulqan, Object)
        public:
        
            static constexpr const uint32_t kMinimumDescriptors = 1000;
            
                //! Polls for events (does not loiter)
            static void poll_events();


                // Creates a window bound to the specified vulkan instance
            Vulqan(const WindowCreateInfo& i = WindowCreateInfo());
            
            ~Vulqan();
            

                //! Calls user's attention to window
            void                attention();

                //! Closes (politely) this window....
            void                close();
            
            VkColorSpaceKHR     color_space() const;
            
            VkCommandBuffer     command_buffer() const;
            
            VkCommandPool       command_pool() const;
            
            VkDescriptorPool    descriptor_pool() const;
            
            VkDevice            device() const;

                //! Brings window to front & input focus
            void                focus();
            
            VkFormat            format() const;

                //! Good & initialized window
            bool                good() const { return m != nullptr; }
            
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
            VkDevice            logical() const;

            
                //! Maximizes widnow
            void                maximize();

                //! Monitor (if fullscreen)
            VqMonitor           monitor() const;

            VkPhysicalDevice    physical() const;

                //! Current window position
            Vector2I            position() const;
            
            VkRenderPass        render_pass() const;
            
                //! Restores the window to non-fullscreen/iconify
            void                restore();

                //! Sets the background color
            void                set_clear(const RGBA4F&);

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

                //! Vulqan size
            Size2I              size() const;
            
            //const VkExtent2D&   swap_extent() const { return m_swapExtent; }
            
                //! The Vulkan surface
            VkSurfaceKHR        surface() const;
            
            VkRect2D            swap_def_scissor() const;
            
            VkViewport          swap_def_viewport() const;

            uint32_t            swap_image_count() const;
            uint32_t            swap_min_image_count() const;

            uint32_t            swap_width() const;
            
            uint32_t            swap_height() const;
            
            
                //! Width of the window
            int                 width() const;

                //! GLFW window handle
            GLFWwindow*         window() const;
            
            //  This is the "DRAW" pass, do it all, whatever the result is
            virtual bool        draw();

            operator VqVisualizer&  () { return *m; }

            void                render(VkCommandBuffer, const Scene&, const Perspective&);

        protected:
        
            friend struct VqVisualizer;
            friend class VqWindow;
        
            //VkPipeline                  m_lastPipeline  = nullptr;
            virtual void        window_resized(){}
            virtual void        viewport_changed(){}
            virtual void        draw_vulqan(VkCommandBuffer){}
            
            
        private:
            std::unique_ptr<VqVisualizer>   m;

            bool    record(VkCommandBuffer, uint32_t);
            
            static void callback_resize(GLFWwindow*, int, int);
        };
        
    }
}
