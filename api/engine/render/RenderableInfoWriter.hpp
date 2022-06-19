////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/meta/ObjectInfoWriter.hpp>
#include <engine/render/Renderable.hpp>

namespace yq {
    namespace engine {
        template <typename C>
        class RenderableInfo::Writer : public ObjectInfo::Writer<C> {
        public:
            Writer(RenderableInfo* renderInfo) : ObjectInfo::Writer<C>(renderInfo)
            {
            }
            
            Writer(RenderableInfo& renderInfo) : Writer(&renderInfo)
            {
            }
        };
    }
}
