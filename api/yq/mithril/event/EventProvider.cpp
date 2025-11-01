////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/event/EventProvider.hpp>
#include <mithril/event/EventCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_events()
    {
        return [](){
            return ids(cdb::all_events(Sorted::YES));
        };
    }
}

