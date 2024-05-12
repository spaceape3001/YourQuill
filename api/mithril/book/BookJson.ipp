////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/book/Book.hpp>
#include <mithril/book/BookCDB.hpp>
#include <mithril/book/BookJson.hpp>

namespace yq::mithril {
    json json_(Book x)
    {
        json j{
            { "id", x.id },
            { "key", cdb::key(x) },
            { "title", cdb::title(x) }
        };
        return j;
    }
    
    json json_(const BookVector& xs)
    {
        json j = json::array();
        for(Book x : xs)
            j.push_back(json_(x));
        return j;
    }
}
