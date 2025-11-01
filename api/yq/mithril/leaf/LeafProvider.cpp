////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/leaf/LeafProvider.hpp>
#include <yq/mithril/leaf/LeafCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_leafs()
    {
        return [](){
            return ids(cdb::all_leafs(Sorted::YES));
        };
    }
}

