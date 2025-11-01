////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/entity/EntityProvider.hpp>
#include <yq/mithril/entity/EntityCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_entities()
    {
        return [](){
            return ids(cdb::all_entities(Sorted::YES));
        };
    }
}

