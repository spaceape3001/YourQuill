////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/meta/TypeInfoWriter.hpp>
#include "UserCDB.hpp"

namespace {
    void reg_user_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<User>();
            w.property("id", READ_ONLY, &User::id);
            w.property("key", (std::string(*)(User)) cdb::key);
        }
    }
}
