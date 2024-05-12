////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/style/Style.hpp>
#include <mithril/style/StyleCDB.hpp>
#include <mithril/style/StyleInfo.hpp>
#include <mithril/style/StyleJson.hpp>

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
