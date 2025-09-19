////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/organization/Organization.hpp>
#include <mithril/organization/OrganizationCDB.hpp>
#include <mithril/organization/OrganizationJson.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Organization x)
    {
        json j{
            { "id", x.id },
            { "key", cdb::key(x) },
            { "name", cdb::name(x) }
        };
        return j;
    }
    
    json json_(const OrganizationVector& xs)
    {
        json j = json::array();
        for(Organization x : xs)
            j.push_back(json_(x));
        return j;
    }
}
