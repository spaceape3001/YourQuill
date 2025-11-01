////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/style/Style.hpp>
#include <yq/mithril/style/StyleCDB.hpp>
#include <yq/mithril/style/StyleInfo.hpp>
#include <yq/mithril/style/StyleJson.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Style x)
    {
        Style::Info  i   = cdb::info(x);
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
    
    json json_(const StyleVector& xs)
    {
        json j = json::array();
        for(Style x : xs)
            j.push_back(json_(x));
        return j;
    }
}
