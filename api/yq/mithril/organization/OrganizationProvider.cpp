////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/organization/OrganizationProvider.hpp>
#include <yq/mithril/organization/OrganizationCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_organizations()
    {
        return [](){
            return ids(cdb::all_organizations(Sorted::YES));
        };
    }
}

