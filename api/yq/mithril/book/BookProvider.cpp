////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/book/BookProvider.hpp>
#include <yq/mithril/book/BookCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_books()
    {
        return [](){
            return ids(cdb::all_books(Sorted::YES));
        };
    }
}

