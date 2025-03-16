////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "EntityCDB.hpp"
#include <yq/meta/TypeInfoWriter.hpp>
#include <yq/meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Entity)
YQ_TYPE_IMPLEMENT(yq::mithril::EntitySet)
YQ_TYPE_IMPLEMENT(yq::mithril::EntityVector)

namespace yq::mithril {
    void reg_entity_meta()
    {
        {
            auto w  = writer<Entity>();
            w.property("id", READ_ONLY, &Entity::id);
            w.property("key", (std::string(*)(Entity)) cdb::key);
        }
    }

    YQ_INVOKE(reg_entity_meta();)
}
