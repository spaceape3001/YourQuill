////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/category/Category.hpp>
#include <yq/mithril/category/CategoryCDB.hpp>
#include <yq/mithril/category/CategoryInfo.hpp>
#include <yq/mithril/category/CategoryJson.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Category x)
    {
        Category::Info  i   = cdb::info(x);
        json j{
            { "brief", i.brief },
            { "document", i.doc.id },
            { "icon", i.icon.id },
            { "id", x.id },
            { "key", i.key },
            { "name", i.name }
        };
        return j;
    }
    
    json json_(const CategoryVector& xs)
    {
        json j = json::array();
        for(Category x : xs)
            j.push_back(json_(x));
        return j;
    }
}
