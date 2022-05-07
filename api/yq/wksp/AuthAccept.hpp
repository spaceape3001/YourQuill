////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Authentication.hpp"

namespace yq {

    namespace auth {
    
        /*! \brief Auto-Accepting Authentication
        
            This will auto-accept ANY password, and is intended for 
            TESTING PURPOSES ONLY!
        */
        class AutoAccept : public Authentication {
            YQ_OBJECT_DECLARE(AutoAccept, Authentication)
        public:
            bool  accept(std::string_view) const override { return true; }
        };
    }
}
