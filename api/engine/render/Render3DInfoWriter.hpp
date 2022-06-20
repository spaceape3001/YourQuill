////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/render/RenderedInfoWriter.hpp>

namespace yq {
    namespace engine {
        template <typename C>
        class Render3DInfo::Writer : public RenderedInfo::Writer<C> {
        public:
            Writer(Render3DInfo* renderInfo) : RenderedInfo::Writer<C>(renderInfo)
            {
            }
            
            Writer(Render3DInfo& renderInfo) : Writer(&renderInfo)
            {
            }
        };
    }
}
