////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/category/CategoryProvider.hpp>
#include <yq/mithril/category/CategoryCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_categories()
    {
        return [](){
            return ids(cdb::all_categories(Sorted::YES));
        };
    }
}

