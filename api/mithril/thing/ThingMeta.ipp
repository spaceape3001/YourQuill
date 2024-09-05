////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/meta/TypeInfoWriter.hpp>
#include "ThingCDB.hpp"

namespace {
    void reg_thing_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Thing>();
            w.property("id", READ_ONLY, &Thing::id);
            w.property("key", (std::string(*)(Thing)) cdb::key);
        }
    }
}
