////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/meta/TypeInfoWriter.hpp>
#include "AttributeCDB.hpp"

namespace {
    void reg_attribute_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Attribute>();
            w.property("children", (AttributeVector(*)(Attribute)) cdb::child_attributes);
            w.property("document", (Document(*)(Attribute)) cdb::document);
            w.property("index", (uint64_t(*)(Attribute)) cdb::index);
            w.property("id", READ_ONLY, &Attribute::id);
            w.property("key", (std::string(*)(Attribute)) cdb::key);
            w.property("name", (std::string(*)(Attribute)) cdb::name);
            w.property("parent", (Attribute(*)(Attribute)) cdb::parent);
            w.property("uid", (uint64_t(*)(Attribute)) cdb::uid);
            w.property("value", (std::string(*)(Attribute)) cdb::value);
        }
    }
}
