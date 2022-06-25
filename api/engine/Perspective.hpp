////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/camera/Camera.hpp>
#include <math/Rectangle2.hpp>
#include <math/RGB.hpp>
#include <optional>

namespace yq {
    namespace engine {
        struct Perspective {
            // Only the camera shall be required
            CameraCPtr                  camera;
            std::optional<RGB3F>        background;
            std::optional<Rectangle2D>  screen;
        };
    }
}
