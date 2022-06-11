////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/Application.hpp>
#include "VqException.hpp"
#include "VqSurface.hpp"
#include <GLFW/glfw3.h>

namespace yq {
    namespace engine {
        VqSurface::VqSurface(GLFWwindow* win)
        {
            if(glfwCreateWindowSurface(Application::vulkan(), win, nullptr, &m_surface) != VK_SUCCESS)
                throw VqException("Unable to create window surface!");
        }
        
        VqSurface::VqSurface(VqSurface&&mv)
        {
            m_surface       = mv.m_surface;
            mv.m_surface    = nullptr;
        }
        
        
        VqSurface::~VqSurface()
        {
            dtor();
        }

        void VqSurface::dtor()
        {
            if(m_surface){
                vkDestroySurfaceKHR(Application::vulkan(), m_surface, nullptr);
                m_surface   = nullptr;
            }
        }

        VqSurface&          VqSurface::operator=(VqSurface&&mv)
        {
            if(this != &mv){
                dtor();
                m_surface       = mv.m_surface;
                mv.m_surface    = nullptr;
            }
            return *this;
        }
    }
}

