////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/place/PlaceProvider.hpp>
#include <yq/mithril/place/PlaceCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_places()
    {
        return [](){
            return ids(cdb::all_places(Sorted::YES));
        };
    }
}

