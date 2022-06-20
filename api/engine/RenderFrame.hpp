////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/Camera.hpp>
#include <engine/Rendered.hpp>

namespace yq {
    namespace engine {
        struct RenderFrame : public RefCount {
            std::vector<CameraPtr>      cameras;
            std::vector<RenderedPtr>    rendereds;
        };
    }
}
