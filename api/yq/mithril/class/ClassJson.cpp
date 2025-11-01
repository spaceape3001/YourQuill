////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/class/ClassCDB.hpp>
#include <mithril/class/ClassInfo.hpp>
#include <mithril/class/ClassJson.hpp>

#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Class x)
    {
        Class::Info i   = cdb::info(x);
        
        json j{
            { "brief", i.brief },
            { "icon", i.icon.id },
            { "id", x.id },
            { "key", i.key },
            { "name", i.name },
            { "plural", i.plural }
        };
        
        if(i.category)
            j["category"]   = i.category.id;
        return j;
    }
    
    json json_(const ClassVector& xs)
    {
        json j = json::array();
        for(Class x : xs)
            j.push_back(json_(x));
        return j;
    }
}
