////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Required.hpp"
#include <basic/Ref.hpp>
#include <basic/DbgRel.hpp>
#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>
#include <vulkan/vulkan_core.h>
#include <set>
#include <vector>

namespace yq {


    class VqWindow;

    /*! Instance for vulkan, should only have ONE
    */
    class VqInstance : trait::not_copyable, trait::not_moveable {
    public:
    
        static const VqInstance*    singleton();
        static VkInstance           vulkan();
        static bool                 initialized();
    
    
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
            Required        validation      = YQ_DBGREL( Required::YES, Required::NO);
            
            
            Info(){}
        };
    
        VqInstance(const Info& i = Info());
        ~VqInstance();
        
        const std::vector<VkExtensionProperties>& available_extensions() const { return m_allExtensionProps; }
        const std::vector<VkLayerProperties>&     available_layers() const { return m_allLayerProps; }
        
        bool    good() const { return m_instance != nullptr; }
        
        VkInstance  instance() const { return m_instance; }
        

    public:
    
        friend class VqWindow;
        
        static VqInstance*          s_singleton;
        static VkInstance           s_vulkan;
        
        bool                        init(const Info&);
        
        VkInstance                          m_instance      = nullptr;
        std::vector<VkLayerProperties>      m_allLayerProps;
        std::vector<VkExtensionProperties>  m_allExtensionProps;
        std::set<std::string>               m_allLayerNames;
        std::set<std::string>               m_allExtensionNames;
        std::vector<const char*>            m_extensions;
        std::vector<const char*>            m_layers;
    };
}

