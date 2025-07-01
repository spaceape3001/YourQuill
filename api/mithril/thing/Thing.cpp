////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ThingCDB.hpp"
#include <yq/meta/TypeMetaWriter.hpp>
#include <yq/meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Thing)
YQ_TYPE_IMPLEMENT(yq::mithril::ThingSet)
YQ_TYPE_IMPLEMENT(yq::mithril::ThingVector)

namespace yq::mithril {
    void reg_thing_meta()
    {
        {
            auto w  = writer<Thing>();
            w.property("id", READ_ONLY, &Thing::id);
            w.property("key", (std::string(*)(Thing)) cdb::key);
        }
    }

    YQ_INVOKE(reg_thing_meta();)
}
