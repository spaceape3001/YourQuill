////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqException.hpp"
#include "VqInternal.hpp"
#include "VqWindow.hpp"
#include <engine/Vulqan.hpp>
#include <GLFW/glfw3.h>

namespace yq {
    namespace engine {
        VqWindow::VqWindow(VqInternal& w, const WindowCreateInfo& wci)
        {
                /*
                    --------------------------------------------------------------------------------------------------------
                    GLFW WINDOW STUFF
                    
                    NOTE, once this has been called, all future aborts will need to call
                    kill() to reset our state
                */

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_FLOATING, wci.floating ? GLFW_TRUE : GLFW_FALSE);
            glfwWindowHint(GLFW_DECORATED, wci.decorated ? GLFW_TRUE : GLFW_FALSE);
            glfwWindowHint(GLFW_RESIZABLE, wci.resizable ? GLFW_TRUE : GLFW_FALSE);

            m_window = glfwCreateWindow(std::max(1,wci.size.width()), std::max(1,wci.size.height()), wci.title, wci.monitor.monitor(), nullptr);
            if(!m_window)
                throw VqException("Unable to create GLFW window");
            
            glfwSetWindowUserPointer(m_window, &w);
            glfwSetWindowSizeCallback(m_window, [](GLFWwindow* gwin, int, int){
                VqInternal    *v  = (VqInternal*) glfwGetWindowUserPointer(gwin);
                if(v){
                    v -> rebuildSwap    = true;
                    if(v->user)
                        v->user->window_resized();
                }
            });
        }
        
        VqWindow::VqWindow(VqWindow&& mv)
        {
            steal(m_window, mv.m_window);
        }
        
        VqWindow& VqWindow::operator=(VqWindow&&mv)
        {
            if(this != &mv){
                dtor();
                steal(m_window, mv.m_window);
            }
            return *this;
        }
        
        VqWindow::~VqWindow()
        {
            dtor();
        }
        
        void    VqWindow::dtor()
        {
            if(!m_window)
                return;
            glfwDestroyWindow(m_window);
            m_window        = nullptr;
        }
    }
}
