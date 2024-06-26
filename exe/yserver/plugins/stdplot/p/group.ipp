////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/group/GroupJson.hpp>
#include <mithril/group/GroupSearch.hpp>

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_group(WebContext& ctx)
        {
            Group    v   = arg::group(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_group_key(WebContext& ctx)
        {
            Group    v   = arg::group(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_groups(WebContext& ctx)
        {
            GroupVector  ret = search(ctx, GROUP);
            return json{
                { "groups", json_(ret) }
            };
        }
        
    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
    
        void p_groups(WebHtml& h)
        {
            //  Search parameters go here... execute the search
            std::vector<Group>  groups   = all_groups(Sorted::YES);
            
            
            //  Form the title... 
            h.title() << "Groups";
            
            h << "<p><i>(Search Bar will go here)</i></p>\n";
            
            //  And results
            
            
            
            dev_table(h, groups);
        }


    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------

        void    p_dev_groups(WebHtml& h)
        {
            h.title() << "All Groups";
            dev_table(h, all_groups(Sorted::YES));
        }

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_group_pages()
        {
            reg_webpage<p_api_group>("/api/group").argument("ID", "Group ID");
            reg_webpage<p_api_group_key>("/api/group/key").argument("ID", "Group ID");
            reg_webpage<p_api_groups>("/api/groups");

            reg_webpage<p_groups>("/groups");
            reg_webpage<p_dev_groups>("/dev/groups");
        }
}
