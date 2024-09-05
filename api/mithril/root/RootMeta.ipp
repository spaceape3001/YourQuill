////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/meta/TypeInfoWriter.hpp>
#include <mithril/root/Root.hpp>
#include <mithril/root/RootCDB.hpp>

namespace {
    void reg_root_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Root>();
            w.property("id", READ_ONLY, &Root::id);
            w.property("key", (std::string(*)(Root)) cdb::key);
        }
    }
}
