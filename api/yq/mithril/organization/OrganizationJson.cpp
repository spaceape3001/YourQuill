////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/organization/Organization.hpp>
#include <yq/mithril/organization/OrganizationCDB.hpp>
#include <yq/mithril/organization/OrganizationJson.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Organization x)
    {
        json j{
            { "brief", cdb::brief(x) },
            { "icon", cdb::icon(x).id },
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
