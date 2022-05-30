////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Ref.hpp>
#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>
#include <math/preamble.hpp>
#include <math/shape/Size2.hpp>
#include <vulkan/vulkan_core.h>
#include <engine/vulqan/VqMonitor.hpp>

struct HelloApp;
struct GLFWwindow;

namespace yq {
    
    class VqPipeline;
    class VqInstance;
    
    /*! \brief A basic window
    
        This encapsulates the GLFW & Vulkan into a single window, requires
        a Vulkan instance.
    */
    class VqWindow : public RefCount, trait::not_copyable, trait::not_moveable {
friend struct HelloApp;
    public:
        
            //! Polls for events (does not loiter)
        static void poll_events();

        struct Info {
            VkPhysicalDevice    device   = nullptr;
        
            const char*         title    = "(untitled)";
            Size2I              size     = { 1920, 1080 };
                //!  Set to get full screen, windowed otherwise
            VqMonitor           monitor;
            VkPresentModeKHR    pmode   = VK_PRESENT_MODE_FIFO_KHR;
            
                //!  Set to make always-on-top
            bool                floating    = false;
            
                //!  Set to make decorated
            bool                decorated   = true;
            
                //!  Set to make user-resizable
            bool                resizable   = false;
            
            Info(){}
        };

            // Creates a window bound to the specified vulkan instance
        VqWindow(const Info& i = Info());
        
        ~VqWindow();

            //! Calls user's attention to window
        void        attention();

            //! Closes (politely) this window....
        void        close();
        
            //! Brings window to front & input focus
        void        focus();

            //! Good & initialized window
        bool        good() const { return m_window != nullptr; }

            //! Height of the window
        int         height() const;
        
            //! Hide the window
        void        hide();

            //! Iconifies (minimizes) window
        void        iconify();

        
            //! TRUE if the window has standard decorations (close/buttons/frame)
        bool        is_decorated() const;
            //! TRUE if the window has input focus
        bool        is_focused() const;
            //! TRUE if window is floating (ie always-on-top)
        bool        is_floating() const;
            //! TRUE if the window is fullscreen
        bool        is_fullscreen() const;
            //! TRUE if the mouse cursor is hovering directly above us
        bool        is_hovered() const;
            //! TRUE if the window is iconified
        bool        is_iconified() const;
            //! TRUE if the window is maximized
        bool        is_maximized() const;
            //! TRUE if the window is resizable
        bool        is_resizable() const;
            //! TRUE if the window is visible
        bool        is_visible() const;
        
        VkDevice    logical() const { return m_logical; }
        
            //! Maximizes widnow
        void        maximize();

            //! Monitor (if fullscreen)
        VqMonitor   monitor() const;

        VkPhysicalDevice    physical() const { return m_physical; }

            //! Current window position
        Vec2I       position() const;
        
            //! Restores the window to non-fullscreen/iconify
        void        restore();

            //! Sets the window position
        void        set_position(const Vec2I&);

            //! Sets the window position
        void        set_position(int x, int y);

            //! Sets window size
        void        set_size(const Size2I&);

            //! Sets window size
        void        set_size(int w, int h);
        
            //! Sets the window title
        void        set_title(const char*);

            //! Sets the window title
        void        set_title(const std::string&);

            //! TRUE if user wants window to close
        bool        should_close() const;
        
            //! Show the window
        void        show();

            //! Window size
        Size2I      size() const;
        
        const VkExtent2D&   swap_extent() const { return m_swapExtent; }
        
            //! The Vulkan surface
        VkSurfaceKHR    surface() const { return m_surface; }
        
            //! Width of the window
        int         width() const;

            //! GLFW window handle
        GLFWwindow* window() const { return m_window; }

        
        
    //private:
        friend class VqPipeline;
        friend struct HelloApp;
        void    _deinit();

        void    _destroy_renderpass();

        void    _destroy_swapviews();
    
            //! Creates & initializes this window
        bool    _init(const Info& i);
        
        bool    _init_physical(VkPhysicalDevice);
        bool    _init_renderpass();
        bool    _init_surface();
        bool    _init_swap();
        bool    _init_swapviews();
        bool    _init_window(const Info&);

        VkPhysicalDevice            m_physical      = nullptr;
        GLFWwindow*                 m_window        = nullptr;
        VkSurfaceKHR                m_surface       = nullptr;
        VkQueue                     m_graphicsQueue = nullptr;
        VkQueue                     m_presentQueue  = nullptr;
        VkDevice                    m_logical       = nullptr;
        VkPresentModeKHR            m_presentMode;
        VkSurfaceFormatKHR          m_surfaceFormat;
        VkSwapchainKHR              m_swapChain     = nullptr;
        VkExtent2D                  m_swapExtent    = { 0, 0 };
        std::vector<VkImage>        m_swapImages;
        std::vector<VkImageView>    m_swapImageViews;
        VkRenderPass                m_renderPass    = nullptr;
    };

}
