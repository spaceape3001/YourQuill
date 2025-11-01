////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/event/EventProvider.hpp>
#include <yq/mithril/event/EventCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_events()
    {
        return [](){
            return ids(cdb::all_events(Sorted::YES));
        };
    }
}

