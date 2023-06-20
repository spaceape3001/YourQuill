////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/book/BookProvider.hpp>
#include <mithril/book/BookCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_books()
    {
        return [](){
            return ids(cdb::all_books(Sorted::YES));
        };
    }
}

