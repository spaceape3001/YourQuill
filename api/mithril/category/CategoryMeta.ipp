////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/meta/TypeInfoWriter.hpp>
#include "CategoryCDB.hpp"

namespace {
    void reg_category_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Category>();
            w.property("brief", (std::string(*)(Category)) cdb::brief);
            w.property("document", (Document(*)(Category)) cdb::document);
            w.property("icon", (Image(*)(Category)) cdb::icon);
            w.property("id", READ_ONLY, &Category::id);
            w.property("key", (std::string(*)(Category)) cdb::key);
            w.property("label", (std::string(*)(Category)) cdb::label);
            w.property("name", (std::string(*)(Category)) cdb::name);
        }
    }
}
