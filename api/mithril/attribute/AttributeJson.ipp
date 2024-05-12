////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/attribute/Attribute.hpp>
#include <mithril/attribute/AttributeCDB.hpp>
#include <mithril/attribute/AttributeJson.hpp>

namespace yq::mithril {
    json json_(Attribute x)
    {
        Attribute::Info  i   = cdb::info(x);
        json j{
            { "id", x.id },
            { "index", i.idx },
            { "key", i.key },
            { "uid", i.uid },
            { "value", i.value }
        };
        
        if(i.doc)
            j["document"]   = i.doc.id;
        if(i.parent)
            j["parent"]     = i.parent.id;
        
        
        return j;
    }
    
    json json_(const AttributeVector& xs)
    {
        json j = json::array();
        for(Attribute x : xs)
            j.push_back(json_(x));
        return j;
    }
}
