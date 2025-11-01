////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/entity/EntityProvider.hpp>
#include <mithril/entity/EntityCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_entities()
    {
        return [](){
            return ids(cdb::all_entities(Sorted::YES));
        };
    }
}

