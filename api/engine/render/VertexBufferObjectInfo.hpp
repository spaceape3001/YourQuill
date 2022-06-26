////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/meta/Meta.hpp>
#include "BufferObjectInfo.hpp"

namespace yq {
    namespace engine {
        class VertexBufferObjectInfo : public BufferObjectInfo {
        public:
            class Writer;
            
            
            
        protected:
            VertexBufferObjectInfo(std::string_view, Pipeline*, const std::source_location&);
        };
    }
}
