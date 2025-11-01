////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/novel/NovelProvider.hpp>
#include <yq/mithril/novel/NovelCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_novels()
    {
        return [](){
            return ids(cdb::all_novels(Sorted::YES));
        };
    }
}

