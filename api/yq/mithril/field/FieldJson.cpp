////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/field/Field.hpp>
#include <yq/mithril/field/FieldCDB.hpp>
#include <yq/mithril/field/FieldInfo.hpp>
#include <yq/mithril/field/FieldJson.hpp>
#include <yq/mithril/image/Image.hpp>
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
