////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/BasicApp.hpp>
#include "EngineCreateInfo.hpp"

struct VkInstance_T;

namespace yq {
    class Window;
    
    class VqApp;
    class VqWidnow;
    
    class EngineApp : public BasicApp {
    public:
    
        static EngineApp*       app() { return s_app; }
        static VkInstance_T*    instance();
    
        EngineApp(int argc, char* argv[], const EngineCreateInfo& ci=EngineCreateInfo());
        ~EngineApp();
        
    private:
        static EngineApp*           s_app;
        static VkInstance_T*        s_instance;

        friend class Window;
        friend class VqWindow;
        friend class VqApp;
        struct Impl;
        std::unique_ptr<Impl>       m;
    };

}
