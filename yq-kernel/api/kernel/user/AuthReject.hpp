////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Authentication.hpp"


namespace yq {
    namespace auth {
        /*! \brief Auto-Rejecting Authentication
        
            This will reject *ALL* attempts to authenticate.  While intended for 
            TESTING PURPOSES, it'll be safe to have in the shipped product.
        */
        class AutoReject : public Authentication {
            YQ_OBJECT_DECLARE(AutoReject, Authentication)
        public:
            bool  accept(std::string_view) const override { return false; }
        };
    }
}
