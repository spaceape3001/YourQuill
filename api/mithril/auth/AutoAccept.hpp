////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Authentication.hpp"

namespace yq::mithril::auth {
    
    /*! \brief Auto-Accepting Authentication
    
        This will auto-accept ANY password, and is intended for 
        TESTING PURPOSES ONLY!
    */
    class AutoAccept : public Authentication {
        YQ_STD_OBJECT_DECLARE(AutoAccept, Authentication)
    public:
        bool  accept(std::string_view) const override { return true; }
        
        AutoAccept();
        ~AutoAccept();

        static void init_info();
    };
}
