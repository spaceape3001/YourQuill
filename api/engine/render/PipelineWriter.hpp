////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Pipeline.hpp"
#include "PipelineBuilder.hpp"
#include <basic/meta/MetaWriter.hpp>

namespace yq {
    namespace engine {
        class Pipeline::Writer : public Meta::Writer, public PipelineBuilder {
        public:
        
            Writer(Pipeline*);
            Writer(Pipeline&);
            ~Writer();
            
        private:
            Pipeline*       m_pipeline  = nullptr;
        };
    }
}
