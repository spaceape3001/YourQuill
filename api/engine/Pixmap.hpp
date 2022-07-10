////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "preamble.hpp"
#include <span>

namespace yq {
    namespace engine {
        class Pixmap : public RefCount {
        public:
        
            DataFormat                      format() const { return m_format; }
            virtual ImageType               image_type() const = 0;
            virtual std::span<std::byte>    data() const = 0;
            virtual size_t                  pixels() const = 0;
            
            virtual size_t                  width() const { return 1ULL; }
            virtual size_t                  height() const { return 1ULL; }
            virtual size_t                  depth() const { return 1ULL; }
            virtual uint8_t                 dims() const = 0;
            
        protected:
            Pixmap(){}
            virtual ~Pixmap(){}
            
            DataFormat                      m_format;
        };
    };
}
