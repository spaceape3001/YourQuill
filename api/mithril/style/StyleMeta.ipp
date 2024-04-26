////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/meta/TypeInfoWriter.hpp>
#include <mithril/style/StyleCDB.hpp>

namespace {
    void reg_style_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Style>();
            w.property("id", READ_ONLY, &Style::id);
            w.property("key", (std::string(*)(Style)) cdb::key);
        }
    }
}
