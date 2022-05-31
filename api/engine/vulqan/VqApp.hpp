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

#include <string>

namespace yq {

    //! Info for initialization
    struct AppInfo {
    
        //! Application name
        std::string     app_name        = "(nameless)";
        
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
    
        static VqApp*           s_app;
        static VkInstance       s_instance;
    
        AppInfo                 m_appInfo;
        VkInstance              m_instance  = nullptr;

        void    init();
        
    };

};
