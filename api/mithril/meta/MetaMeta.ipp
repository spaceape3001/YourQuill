////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/meta/TypeInfoWriter.hpp>
#include "MetaCDB.hpp"

namespace {
    void reg_meta_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<yq::mithril::Meta>();
            w.property("id", READ_ONLY, &yq::mithril::Meta::id);
            w.property("key", (std::string(*)(yq::mithril::Meta)) cdb::key);
        }
    }
}
