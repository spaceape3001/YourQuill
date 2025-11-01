////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/thing/ThingProvider.hpp>
#include <yq/mithril/thing/ThingCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_things()
    {
        return [](){
            return ids(cdb::all_things(Sorted::YES));
        };
    }
}

