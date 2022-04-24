////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {

    class SimpleAuthentication : public Authentication {
        YQ_OBJECT_DECLARE(SimpleAuthentication, Authentication)
    public:
        bool  accept(std::string_view k) const override 
        { 
            return k == (std::string_view) m_password; 
        }
        
        std::string     m_password;
    };
    
    YQ_INVOKE( 
        auto w = writer<SimpleAuthentication>();
        
        w.label("Simple");
        w.description("For Testing Purposes, UNECRYPTED password authentication");
        w.property("password", &SimpleAuthentication::m_password);
    )
    
}

YQ_OBJECT_IMPLEMENT(yq::SimpleAuthentication)

