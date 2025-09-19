////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/value/Value.hpp>
#include <mithril/value/ValueCDB.hpp>
#include <mithril/value/ValueJson.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Value x)
    {
        Value::Info  i   = cdb::info(x);
        json j{
            { "brief", i.brief },
            { "data", i.data },
            { "field", i.field.id },
            { "id", x.id },
            { "key", cdb::key(x) }
        };
        return j;
    }
    
    json json_(const ValueVector& xs)
    {
        json j = json::array();
        for(Value x : xs)
            j.push_back(json_(x));
        return j;
    }
}
