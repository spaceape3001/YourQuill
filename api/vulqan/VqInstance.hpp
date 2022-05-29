////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Ref.hpp>
#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>
#include <vulkan/vulkan_core.h>
#include <vector>

namespace yq {

    enum class Required {
        NO,
        OPTIONAL,
        YES
    };

    class VqWindow;

    /*! Instance for vulkan, should only have ONE
    */
    class VqInstance : public RefCount, trait::not_copyable, trait::not_moveable {
    public:
    
    
        //! Info for initialization
        struct Info {
        
            //! Application name
            const char*     app_name        = "(nameless)";
            
            //! Engine Name
            const char*     engine_name     = "(nameless)";
            
            //! Application version number
            uint32_t        app_version     = 0;
            
            //! Engine version number
            uint32_t        engine_version  = 0;
            
            //! Vulkan API version
            uint32_t        vulkan_api      = VK_API_VERSION_1_2;
            
            //! Add KHRONOS validation layer
            Required        validation      = Required::NO;
            
            //! TRUE to enable glfw extensions
            bool            glfw_enable     = true;
            
            //! TRUE to initialize glfw
            bool            glfw_init       = true;
            
            Info(){}
        };
    
        VqInstance();
        VqInstance(const Info&);
        ~VqInstance();
        
        bool                initialize(const Info& i=Info());
        void                shutdown();
        
        VkPhysicalDevice    physical_device() const { return m_physDevice; }
        VkInstance          instance() const { return m_instance; }
        bool                good() const { return m_init; }

    public:
    
        friend class VqWindow;
    
        //!  Unabashed deinitializes... 
        void            _deinit();
    
        std::vector<const char*>    m_layers;
        VkInstance                  m_instance      = nullptr;
        VkPhysicalDevice            m_physDevice    = nullptr;
        bool                        m_glfw          = false;
        bool                        m_init          = false;        
    };
}

