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

    Camera* CameraInfo::createC(DObject* parent) const
    {
        return static_cast<Camera*>(createD(parent));
    }
    
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////


    Camera::Camera(DObject* parent) : DObject(parent)
    {
    }
    
    Camera::Camera(const Camera&cp) : DObject(cp)
    {
    }
    
    Camera::~Camera()
    {
    }



    //! Remap IDs/pointers appropriately
    void        Camera::remap(const Remapper&rMap)
    {
        DObject::remap(rMap);
    }
}

YQ_OBJECT_IMPLEMENT(yq::doodle::Camera)
