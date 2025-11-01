////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/organization/Organization.hpp>
#include <yq/mithril/organization/OrganizationCDB.hpp>
#include <yq/mithril/organization/OrganizationSearch.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    OrganizationVector  search(WebContext&ctx, organization_k)
    {
        WebSearcher<Organization>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_organizations(Sorted::YES);
        return ws.data;
    }
}
