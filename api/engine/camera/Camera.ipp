////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

//#pragma once

#include <engine/camera/Camera.hpp>
#include <engine/camera/CameraProxy.hpp>
#include <engine/camera/CameraInfoWriter.hpp>
#include <basic/DelayInit.hpp>
#include <basic/meta/Init.hpp>

namespace yq {
    namespace engine {

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        CameraInfo::CameraInfo(std::string_view name, ObjectInfo& base, const std::source_location& sl) : 
            ObjectInfo(name, base, sl)
        {
            set_option(CAMERA);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        bool CameraProxy::operator==(const CameraProxy&) const noexcept = default;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Camera::Camera()
        {
        }

        Camera::~Camera()
        {
        }

        CameraProxy     Camera::proxy(const CameraParams&p) const
        {
            return { id(), revision(), world2screen(p) };
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        YQ_INVOKE(
            auto cam   = writer<Camera>();
            cam.property("id", &Camera::id);
        )
    }
}

YQ_OBJECT_IMPLEMENT(yq::engine::Camera)