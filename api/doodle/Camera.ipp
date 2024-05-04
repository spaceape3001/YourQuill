////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Camera.hpp"

namespace yq::doodle {
    CameraInfo::CameraInfo(std::string_view zName, const DObjectInfo& base, const std::source_location& sl) : DObjectInfo(zName, base, sl)
    {
        set(Flag::CAMERA);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    Camera::Camera(Project&prj) : DObject(prj)
    {
    }
    
    Camera::Camera(Project& prj, const Camera&cp) : DObject(prj, cp)
    {
    }
    
    Camera::~Camera()
    {
    }

    void        Camera::remap(const Remapper&rMap)
    {
        DObject::remap(rMap);
    }
}

