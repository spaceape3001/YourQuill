////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqGLFW.hpp"
#include <basic/Logging.hpp>

namespace yq {
    VqGLFW* VqGLFW::s_singleton = nullptr;

    typedef void (*FNErrorCallback)(int, const char*);

    static void    glfwLogging(int ec, const char* why)
    {
        static const FNErrorCallback  prior = glfwSetErrorCallback(glfwLogging);
        if(ec){
            if(!why)
                why = "Unknown error";
            yWarning() << "GLFW error (" << ec << "): " << why;
        }
        if(prior)
            prior(ec, why);
    }
    

    VqGLFW::VqGLFW()
    {
        if(!s_singleton){
            glfwLogging(0, nullptr);
            glfwInit();
            s_singleton  = this;
        }
    }
    
    VqGLFW::~VqGLFW()
    {
        if(s_singleton == this){
            glfwTerminate();
            s_singleton  = nullptr;
        }
    }
        
    bool     VqGLFW::initialized()
    {
        return s_singleton != nullptr;
    }
}

