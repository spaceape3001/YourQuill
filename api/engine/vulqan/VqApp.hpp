////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Required.hpp"

#include <basic/BasicApp.hpp>
#include <basic/DbgRel.hpp>

#include <vulkan/vulkan_core.h>

#include <set>
#include <string>
#include <vector>

namespace yq {
    class VqWindow;

    //! Info for initialization
    struct AppInfo {
    
        //! Application name
        std::string     app_name;
        
        //! Engine Name
        std::string     engine_name     = "(nameless)";
        
        //! Application version number
        uint32_t        app_version     = 0;
        
        //! Engine version number
        uint32_t        engine_version  = 0;
        
        //! Vulkan API version
        uint32_t        vulkan_api      = VK_API_VERSION_1_2;
        
        //! Add KHRONOS validation layer
        Required        validation      = YQ_DBGREL( Required::YES, Required::NO);
        
        
        AppInfo(){}
    };
    
    class VqApp : public BasicApp {
    public:
        VqApp(int argc, char* argv[], const AppInfo& ai=AppInfo());
        ~VqApp();
        
        static VqApp*           app() { return s_app; }
        static VkInstance       instance();
        
    private:
    
        friend class VqWindow;
    
        static VqApp*     s_app;
        static VkInstance s_instance;
    
        AppInfo                             m_appInfo;
        VkInstance                          m_instance  = nullptr;
        bool                                m_glfw      = false;
        std::vector<VkLayerProperties>      m_allLayerProps;
        std::vector<VkExtensionProperties>  m_allExtensionProps;
        std::set<std::string>               m_allLayerNames;
        std::set<std::string>               m_allExtensionNames;
        std::vector<const char*>            m_extensions;
        std::vector<const char*>            m_layers;

        bool    init();
        void    deinit();
    };

};
