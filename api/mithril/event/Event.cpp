////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "EventCDB.hpp"
#include <yq/meta/TypeInfoWriter.hpp>
#include <yq/meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Event)
YQ_TYPE_IMPLEMENT(yq::mithril::EventSet)
YQ_TYPE_IMPLEMENT(yq::mithril::EventVector)

namespace yq::mithril {
    void reg_event_meta()
    {
        {
            auto w  = writer<Event>();
            w.property("id", READ_ONLY, &Event::id);
            w.property("key", (std::string(*)(Event)) cdb::key);
        }
    }

    YQ_INVOKE(reg_event_meta();)
}
