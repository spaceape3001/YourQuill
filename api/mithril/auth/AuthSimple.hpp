////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Authentication.hpp"

namespace yq::mithril::auth {
    /*! \brief Simple PLAIN TEXT password authentication
    
        This is for testing purposes until a more solid password authentication 
        is chosen.  Until then, it'll be here, after that, TESTING purposes only
        and DISABLED in shipped releases.
    */
    class Simple : public Authentication {
        YQ_OBJECT_DECLARE(Simple, Authentication)
    public:
        bool  accept(std::string_view k) const override 
        { 
            return k == (std::string_view) m_password; 
        }
        
        std::string     m_password;
    };
}
