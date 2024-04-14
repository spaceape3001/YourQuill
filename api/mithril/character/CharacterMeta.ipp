////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/meta/TypeInfoWriter.hpp>
#include "CharacterCDB.hpp"

namespace {
    void reg_character_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Character>();
            w.property("atom", (Atom(*)(Character)) cdb::atom);
            w.property("entity", (Entity(*)(Character)) cdb::entity);
            w.property("icon", (Image(*)(Character)) cdb::icon);
            w.property("id", READ_ONLY, &Character::id);
            w.property("key", (std::string(*)(Character)) cdb::key);
            w.property("name", (std::string(*)(Character)) cdb::name);
        }
    }
}
