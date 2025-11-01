////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "CharacterCDB.hpp"
#include <yq/meta/TypeMetaWriter.hpp>
#include <yq/meta/Init.hpp>
#include <mithril/atom/Atom.hpp>
#include <mithril/entity/Entity.hpp>
#include <mithril/image/Image.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Character)
YQ_TYPE_IMPLEMENT(yq::mithril::CharacterSet)
YQ_TYPE_IMPLEMENT(yq::mithril::CharacterVector)

namespace yq::mithril {
    void reg_character_meta()
    {
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
    YQ_INVOKE(reg_character_meta();)
}
