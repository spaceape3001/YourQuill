////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/Camera.hpp>

namespace yq {
    namespace asset {
        class NullCamera : public Camera {
            YQ_OBJECT_DECLARE(NullCamera, Camera)
        public:
        
            glm::dmat4  world2screen(const CameraParams&) const override;
            NullCamera();
            ~NullCamera();
        };
    }
}
