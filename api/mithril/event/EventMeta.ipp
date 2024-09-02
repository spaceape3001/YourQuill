////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/TypeInfoWriter.hpp>
#include "EventCDB.hpp"

namespace {
    void reg_event_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Event>();
            w.property("id", READ_ONLY, &Event::id);
            w.property("key", (std::string(*)(Event)) cdb::key);
        }
    }
}
