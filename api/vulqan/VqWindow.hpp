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

struct GLFWwindow;

namespace yq {
    
    class VqInstance;
    
    /*! \brief A basic window
    
        This encapsulates the GLFW & Vulkan into a single window, requires
        a Vulkan instance.
    */
    class VqWindow : public RefCount, trait::not_copyable, trait::not_moveable {
    public:
    
        struct Info {
            const char*         title    = "(untitled)";
            Size2I              size     = { 1920, 1080 };
            bool                is_full  = false;
            
            Info(){}
        };

            // Creates a window bound to the specified vulkan instance
        VqWindow(Ref<VqInstance>);
        VqWindow(Ref<VqInstance>, const Info&);
        
        ~VqWindow();
        
        bool        initialize(const Info& i=Info());
        bool        good() const { return m_init; }
        GLFWwindow* window() const { return m_window; }
        void        close();
        
        bool        should_close() const;
        
        static void poll_events();
        
    private:
        void    _deinit();
    
        Ref<VqInstance> m_vulkan;
        GLFWwindow*     m_window        = nullptr;
        VkSurfaceKHR    m_surface       = nullptr;
        VkQueue         m_graphicsQueue = nullptr;
        VkQueue         m_presentQueue  = nullptr;
        VkDevice        m_logicalDevice = nullptr;
        bool            m_init          = false;
    };

}
