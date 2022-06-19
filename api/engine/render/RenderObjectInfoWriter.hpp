////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/render/RenderableInfoWriter.hpp>

namespace yq {
    namespace engine {
        template <typename C>
        class RenderObjectInfo::Writer : public RenderableInfo::Writer<C> {
        public:
            Writer(RenderObjectInfo* renderInfo) : RenderableInfo::Writer<C>(renderInfo)
            {
            }
            
            Writer(RenderObjectInfo& renderInfo) : Writer(&renderInfo)
            {
            }
        };
    }
}
