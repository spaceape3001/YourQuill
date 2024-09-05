////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/meta/TypeInfoWriter.hpp>
#include "NovelCDB.hpp"

namespace {
    void reg_novel_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Novel>();
            w.property("id", READ_ONLY, &Novel::id);
            w.property("key", (std::string(*)(Novel)) cdb::key);
        }
    }
}
