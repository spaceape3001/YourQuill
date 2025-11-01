////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/category/CategoryProvider.hpp>
#include <mithril/category/CategoryCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_categories()
    {
        return [](){
            return ids(cdb::all_categories(Sorted::YES));
        };
    }
}

