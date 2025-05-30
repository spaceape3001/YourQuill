////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/organization/Organization.hpp>
#include <mithril/organization/OrganizationCDB.hpp>
#include <mithril/organization/OrganizationSearch.hpp>
#include <mithril/web/WebSearcher.hpp>

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
