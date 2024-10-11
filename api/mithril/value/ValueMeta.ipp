////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/TypeInfoWriter.hpp>
#include "ValueCDB.hpp"

namespace {
    void reg_value_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Value>();
            w.property("id", READ_ONLY, &Value::id);
            w.property("key", (std::string(*)(Value)) cdb::key);
        }
    }
}
