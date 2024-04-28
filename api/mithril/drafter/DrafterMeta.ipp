////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/meta/TypeInfoWriter.hpp>
#include <mithril/drafter/DrafterCDB.hpp>

namespace {
    void reg_drafter_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Drafter>();
            w.property("id", READ_ONLY, &Drafter::id);
            w.property("key", (std::string(*)(Drafter)) cdb::key);
        }
    }
}
