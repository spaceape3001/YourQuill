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
        class IndexBufferObjectInfo : public BufferObjectInfo {
        public:
            class Writer;
            
            
        protected:
            IndexBufferObjectInfo(std::string_view, Pipeline*, const std::source_location&);
        };
    }
}
