////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/meta/ObjectInfoWriter.hpp>
#include <engine/camera/Camera.hpp>

namespace yq {
    namespace engine {
        template <typename C>
        class CameraInfo::Writer : public ObjectInfo::Writer<C> {
        public:
            Writer(CameraInfo* renderInfo) : ObjectInfo::Writer<C>(renderInfo)
            {
            }
            
            Writer(CameraInfo& renderInfo) : Writer(&renderInfo)
            {
            }
        };
    }
}
