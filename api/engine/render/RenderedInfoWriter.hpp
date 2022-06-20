////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/meta/ObjectInfoWriter.hpp>
#include <engine/render/Rendered.hpp>

namespace yq {
    namespace engine {
        template <typename C>
        class RenderedInfo::Writer : public ObjectInfo::Writer<C> {
        public:
            Writer(RenderedInfo* renderInfo) : ObjectInfo::Writer<C>(renderInfo)
            {
            }
            
            Writer(RenderedInfo& renderInfo) : Writer(&renderInfo)
            {
            }
        };
    }
}
