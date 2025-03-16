////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/category/Category.hpp>
#include <mithril/category/CategoryCDB.hpp>
#include <mithril/category/CategoryInfo.hpp>
#include <mithril/category/CategoryJson.hpp>
#include <nlohmann/json.hpp>

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
