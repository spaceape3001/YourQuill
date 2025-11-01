////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/tag/TagProvider.hpp>
#include <yq/mithril/tag/TagCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_tags()
    {
        return [](){
            return ids(cdb::all_tags(Sorted::YES));
        };
    }
}

