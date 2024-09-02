////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/TypeInfoWriter.hpp>
#include "ClassCDB.hpp"

namespace {
    void reg_class_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Class>();
            w.property("binding", (std::string(*)(Class)) cdb::binding);
            w.property("brief", (std::string(*)(Class)) cdb::brief);
            w.property("category", (Category(*)(Class)) cdb::category);
            w.property("document", (Document(*)(Class)) cdb::document);
            w.property("icon", (Image(*)(Class)) cdb::icon);
            w.property("id", READ_ONLY, &Class::id);
            w.property("key", (std::string(*)(Class)) cdb::key);
            w.property("label", (std::string(*)(Class)) cdb::label);
            w.property("name", (std::string(*)(Class)) cdb::name);
            w.property("plural", (std::string(*)(Class)) cdb::plural);
            w.property("url", (std::string(*)(Class)) cdb::url);
        }
    }
}
