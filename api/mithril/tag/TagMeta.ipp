////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/TypeInfoWriter.hpp>
#include "TagCDB.hpp"

namespace {
    void reg_tag_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Tag>();
            w.property("id", READ_ONLY, &Tag::id);
            w.property("key", (std::string(*)(Tag)) cdb::key);
        }
    }
}
