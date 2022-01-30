////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TypeInfo.hpp"

namespace yq {
    namespace meta {
    
        /*! \brief Shared pointer to *whatever"
        */
        class PointerInfo : public TypeInfo {  // TODO (Figure this out)
        public:
        
            const Meta&         to() const { return m_to; }
            
        protected:
            //PointerInfo(const 
            
        private:
            const Meta&         m_to;
        };
    }
}
