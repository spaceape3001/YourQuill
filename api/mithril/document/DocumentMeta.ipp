////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/meta/TypeInfoWriter.hpp>
#include "DocumentCDB.hpp"
#include <mithril/attribute/AttributeCDB.hpp>

namespace {
    void reg_document_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Document>();
            w.property("attributes", (AttributeVector(*)(Document)) cdb::attributes);
            w.property("base", (std::string(*)(Document)) cdb::base_key);
            w.property("id", READ_ONLY, &Document::id);
            w.property("key", (std::string(*)(Document)) cdb::key);
            w.property("skey", (std::string(*)(Document)) cdb::skey);
            w.property("skeyb", (std::string(*)(Document)) cdb::skeyb);
            w.property("skeyc", (std::string(*)(Document)) cdb::skeyc);
        }
    }
}
