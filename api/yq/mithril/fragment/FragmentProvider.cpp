////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/fragment/FragmentProvider.hpp>
#include <yq/mithril/fragment/FragmentCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_fragments()
    {
        return [](){
            return ids(cdb::all_fragments(Sorted::YES));
        };
    }
}

