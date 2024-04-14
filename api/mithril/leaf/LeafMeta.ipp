////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/meta/TypeInfoWriter.hpp>
#include "LeafCDB.hpp"

namespace {
    void reg_leaf_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Leaf>();
            w.property("id", READ_ONLY, &Leaf::id);
            w.property("key", (std::string(*)(Leaf)) cdb::key);
        }
    }
}
