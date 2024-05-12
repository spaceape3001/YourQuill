////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/thing/Thing.hpp>
#include <mithril/thing/ThingCDB.hpp>
#include <mithril/thing/ThingJson.hpp>

namespace yq::mithril {
    json json_(Thing x)
    {
        json j{
            { "id", x.id },
            { "key", cdb::key(x) },
            { "name", cdb::name(x) }
        };
        return j;
    }
    
    json json_(const ThingVector& xs)
    {
        json j = json::array();
        for(Thing x : xs)
            j.push_back(json_(x));
        return j;
    }
}
