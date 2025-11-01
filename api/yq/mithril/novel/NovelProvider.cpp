////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/novel/NovelProvider.hpp>
#include <mithril/novel/NovelCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_novels()
    {
        return [](){
            return ids(cdb::all_novels(Sorted::YES));
        };
    }
}

