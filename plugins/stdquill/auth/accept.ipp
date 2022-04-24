////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {

    class AcceptAuthentication : public Authentication {
        YQ_OBJECT_DECLARE(AcceptAuthentication, Authentication)
    public:
        bool  accept(std::string_view) const override { return true; }
    };
    
    YQ_INVOKE( 
        writer<AcceptAuthentication>().label("Accept").description("For Testing Purposes, auto-accepting authentication");
    )
    
}

YQ_OBJECT_IMPLEMENT(yq::AcceptAuthentication)

