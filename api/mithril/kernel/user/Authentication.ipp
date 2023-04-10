////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Authentication.hpp"

#include "AuthAccept.hpp"
#include "AuthReject.hpp"
#include "AuthSimple.hpp"

#include <meta/ObjectInfoWriter.hpp>


YQ_OBJECT_IMPLEMENT(yq::mithril::Authentication)
YQ_OBJECT_IMPLEMENT(yq::mithril::auth::AutoAccept)
YQ_OBJECT_IMPLEMENT(yq::mithril::auth::AutoReject)
YQ_OBJECT_IMPLEMENT(yq::mithril::auth::Simple)


namespace yq::mithril {        
    YQ_INVOKE( 
        auto w = writer<auth::Simple>();
        w.label("Simple");
        w.description("For Testing Purposes, UNECRYPTED password authentication");
        w.property("password", &auth::Simple::m_password);

        writer<auth::AutoAccept>().label("Accept").description("For Testing Purposes, auto-accepting authentication");
        writer<auth::AutoReject>().label("Reject").description("For Testing Purposes, auto-rejecting authentication");
    )
}
