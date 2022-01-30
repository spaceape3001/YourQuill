////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Meta.hpp"

namespace yq {
    namespace meta {
        /*! \brief Information about an argument
        */
        class ArgInfo : public Meta {
        public:
        
            const TypeInfo&        type() const { return m_type; }
        
        private:
            ArgInfo(const char* zName, const TypeInfo&, Meta*);
            
            const TypeInfo&         m_type;
            impl::DataBlock         m_default;
        };
    }
}
