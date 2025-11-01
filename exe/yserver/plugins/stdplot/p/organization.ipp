////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/organization/OrganizationJson.hpp>
#include <yq/mithril/organization/OrganizationSearch.hpp>

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_organization(WebContext& ctx)
        {
            Organization    v   = arg::organization(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_organization_key(WebContext& ctx)
        {
            Organization    v   = arg::organization(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_organizations(WebContext& ctx)
        {
            OrganizationVector  ret = search(ctx, ORGANIZATION);
            return json{
                { "organizations", json_(ret) }
            };
        }
        
    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
    
    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------

        void    p_dev_organizations(WebHtml& h)
        {
            h.title() << "All Organizations";
            dev_table(h, all_organizations(Sorted::YES));
        }

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_organization_pages()
        {
            reg_webpage<p_api_organization>("/api/organization").argument("ID", "Organization ID");
            reg_webpage<p_api_organization_key>("/api/organization/key").argument("ID", "Organization ID");
            reg_webpage<p_api_organizations>("/api/organizations");

            reg_webpage<p_dev_organizations>("/dev/organizations");
            reg_webtemplate("/organizations", wksp::shared("www/organizations.ht"sv));
        }
}
