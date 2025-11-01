////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/book/Book.hpp>
#include <yq/mithril/book/BookCDB.hpp>
#include <yq/mithril/book/BookJson.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Book x)
    {
        json j{
            { "brief", cdb::brief(x) },
            { "icon", cdb::icon(x).id },
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
