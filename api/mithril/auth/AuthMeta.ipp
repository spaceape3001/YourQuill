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

#include <yq-toolbox/meta/ObjectInfoWriter.hpp>


namespace  {        
    void    reg_authentication_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w = writer<auth::Simple>();
            w.label("Simple");
            w.description("For Testing Purposes, UNECRYPTED password authentication");
            w.property("password", &auth::Simple::m_password);
        }
        
        {
            auto w = writer<auth::AutoAccept>();
            w.label("Accept");
            w.description("For Testing Purposes, auto-accepting authentication");
        }
        
        {
            auto w = writer<auth::AutoReject>();
            w.label("Reject");
            w.description("For Testing Purposes, auto-rejecting authentication");
        }
    }
}
