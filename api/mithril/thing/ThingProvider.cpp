////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/thing/ThingProvider.hpp>
#include <mithril/thing/ThingCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_things()
    {
        return [](){
            return ids(cdb::all_things(Sorted::YES));
        };
    }
}

