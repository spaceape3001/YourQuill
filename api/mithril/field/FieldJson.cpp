////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/field/Field.hpp>
#include <mithril/field/FieldCDB.hpp>
#include <mithril/field/FieldInfo.hpp>
#include <mithril/field/FieldJson.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Field x)
    {
        Field::Info  i   = cdb::info(x);
        json j{
            { "brief", i.brief },
            { "category", i.category.id },
            { "class", i.class_.id },
            { "document", cdb::document(x).id },
            { "icon", i.icon.id },
            { "id", x.id },
            { "key", i.key },
            { "max", i.max_count },
            { "multiplicity", i.multiplicity.key() },
            { "name", i.name },
            { "pkey", i.pkey },
            { "plural", i.plural },
            { "restriction", i.restriction.key() }
        };
        return j;
    }
    
    json json_(const FieldVector& xs)
    {
        json j = json::array();
        for(Field x : xs)
            j.push_back(json_(x));
        return j;
    }
}
