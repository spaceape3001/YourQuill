////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/TypeInfoWriter.hpp>
#include "FragmentCDB.hpp"

namespace {
    void reg_fragment_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Fragment>();
            w.property("id", READ_ONLY, &Fragment::id);
            w.property("key", (std::string(*)(Fragment)) cdb::key);
        }
    }
}
