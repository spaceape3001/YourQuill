////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "AutoReject.hpp"
#include <yq/meta/ObjectInfoWriter.hpp>

YQ_STD_OBJECT_IMPLEMENT(yq::mithril::auth::AutoReject)

namespace yq::mithril::auth {
    AutoReject::AutoReject(){}
    AutoReject::~AutoReject(){}

    void AutoReject::init_meta()
    {
        auto w = writer<AutoReject>();
        w.label("Reject");
        w.description("For Testing Purposes, auto-rejecting authentication");
    }
}
