////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <string>
#include <math/preamble.hpp>
#include <math/ColorRgb.hpp>
#include <math/shape/Size2.hpp>

struct GLFWmonitor;
struct GLFWvidmode;

namespace yq {

    struct VqVidMode {
        Size2I      size            = {};
        ColorRgbI   bits            = {};
        int         refresh_rate    = 0;
        VqVidMode(){}
        VqVidMode(const GLFWvidmode&);
    };
    
    class VqWindow;

    class VqMonitor {
    public:
        static std::vector<VqMonitor>   enumerate();
        
        //! Returns the primary monitor
        static VqMonitor                primary();

        VqMonitor(){}
        
        //! Underlying GLFW Monitor pointer
        GLFWmonitor*            monitor() const { return m_monitor; }
    
        //! Name of the monitor
        std::string             name() const;
        
        //! Position of the monitor
        Vec2I                   position() const;
        
        //! Physical size of monitor (in MM)
        Size2I                  phys_size_mm() const;
        
        //! Scale of the montior
        Vec2F                   scale() const;
        
        //! Current video mode
        VqVidMode               video_mode_current() const;
        
        //! All video modes
        std::vector<VqVidMode>  video_modes_available() const;
    
        /*! Monitor's "Work area"
        
            This work area is returned in screen coordinates.
        */
        Rect2I                  work_area() const;
        
        constexpr operator bool () const noexcept { return m_monitor != nullptr; }
        constexpr bool    operator==(const VqMonitor&) const noexcept = default;
    
    private:
        friend class VqWindow;
        GLFWmonitor *m_monitor = nullptr;
        VqMonitor(GLFWmonitor *m) : m_monitor(m) {}
    };

}
