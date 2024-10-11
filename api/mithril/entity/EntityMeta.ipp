////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/TypeInfoWriter.hpp>
#include "EntityCDB.hpp"

namespace {
    void reg_entity_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Entity>();
            w.property("id", READ_ONLY, &Entity::id);
            w.property("key", (std::string(*)(Entity)) cdb::key);
        }
    }
}
