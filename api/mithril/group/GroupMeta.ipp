////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/meta/TypeInfoWriter.hpp>
#include "GroupCDB.hpp"

namespace {
    void reg_group_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Group>();
            w.property("id", READ_ONLY, &Group::id);
            w.property("key", (std::string(*)(Group)) cdb::key);
        }
    }
}
