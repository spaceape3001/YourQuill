////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "AutoAccept.hpp"
#include <yq/meta/ObjectInfoWriter.hpp>

YQ_STD_OBJECT_IMPLEMENT(yq::mithril::auth::AutoAccept)

namespace yq::mithril::auth {
    AutoAccept::AutoAccept(){}
    AutoAccept::~AutoAccept(){}

    void AutoAccept::init_info()
    {
        auto w = writer<AutoAccept>();
        w.label("Accept");
        w.description("For Testing Purposes, auto-accepting authentication");
    }
}
