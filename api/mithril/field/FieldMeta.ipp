////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/TypeInfoWriter.hpp>
#include "FieldCDB.hpp"

namespace {
    void reg_field_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Field>();
            w.property("id", READ_ONLY, &Field::id);
            w.property("key", (std::string(*)(Field)) cdb::key);
        }
    }
}
