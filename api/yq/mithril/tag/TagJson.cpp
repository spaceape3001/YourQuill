////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/tag/Tag.hpp>
#include <yq/mithril/tag/TagCDB.hpp>
#include <yq/mithril/tag/TagJson.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Tag x)
    {
        Tag::Info  i   = cdb::info(x);
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
    
    json json_(const TagVector& xs)
    {
        json j = json::array();
        for(Tag x : xs)
            j.push_back(json_(x));
        return j;
    }
}
