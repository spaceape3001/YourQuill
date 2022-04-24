////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {

    class RejectAuthentication : public Authentication {
        YQ_OBJECT_DECLARE(RejectAuthentication, Authentication)
    public:
        bool  accept(std::string_view) const override { return false; }
    };
    
    YQ_INVOKE( 
        writer<RejectAuthentication>().label("Reject").description("For Testing Purposes, auto-rejecting authentication");
    )
    
}

YQ_OBJECT_IMPLEMENT(yq::RejectAuthentication)

