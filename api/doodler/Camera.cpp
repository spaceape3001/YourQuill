////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Camera.hpp"
#include "CameraInfoWriter.hpp"

YQ_OBJECT_IMPLEMENT(yq::doodler::Camera)

namespace yq::doodler {
    CameraInfo::CameraInfo(std::string_view zName, DObjectInfo& base, const std::source_location& sl) : DObjectInfo(zName, base, sl)
    {
        set(Flag::CAMERA);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    void Camera::init_info()
    {
        auto w = writer<Camera>();
        w.description("Camera");
    }

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

