////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/meta/TypeInfoWriter.hpp>
#include "OrganizationCDB.hpp"

namespace {
    void reg_organization_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Organization>();
            w.property("id", READ_ONLY, &Organization::id);
            w.property("key", (std::string(*)(Organization)) cdb::key);
        }
    }
}
