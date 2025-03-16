////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/fragment/FragmentProvider.hpp>
#include <mithril/fragment/FragmentCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_fragments()
    {
        return [](){
            return ids(cdb::all_fragments(Sorted::YES));
        };
    }
}

