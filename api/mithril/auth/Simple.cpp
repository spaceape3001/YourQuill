////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Simple.hpp"
#include <yq/meta/ObjectInfoWriter.hpp>

YQ_STD_OBJECT_IMPLEMENT(yq::mithril::auth::Simple)

namespace yq::mithril::auth {
    Simple::Simple(){}
    Simple::~Simple(){}

    void Simple::init_meta()
    {
        auto w = writer<Simple>();
        w.label("Simple");
        w.description("For Testing Purposes, UNECRYPTED password authentication");
        w.property("password", &auth::Simple::m_password);
    }
}
