////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Authentication.hpp"
#include <yq/meta/ObjectInfoWriter.hpp>

YQ_OBJECT_IMPLEMENT(yq::mithril::Authentication)

namespace yq::mithril {
    
    Authentication::Authentication()
    {
    }
    
    Authentication::~Authentication()
    {
    }

    void Authentication::init_info()
    {
        auto w = writer<Authentication>();
        w.description("Authentication Helper");
    }
}

