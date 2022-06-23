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
        /*! \brief Frame of things to be rendered, both cameras & items.
        
        *//*
            To get things to be rendered, the general process should go...
            
            RenderFrame -> ProxyFrame -> VqObjectFrame -> VqCommands
        */
        struct RenderFrame : public RefCount {
            std::vector<CameraPtr>      cameras;
            std::vector<RenderedPtr>    rendereds;
        };
    }
}
