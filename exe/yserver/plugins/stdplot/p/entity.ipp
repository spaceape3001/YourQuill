////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/entity/EntityJson.hpp>
#include <yq/mithril/entity/EntitySearch.hpp>

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_entity(WebContext& ctx)
        {
            Entity    v   = arg::entity(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_entity_key(WebContext& ctx)
        {
            Entity    v   = arg::entity(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_entities(WebContext& ctx)
        {
            EntityVector  ret = search(ctx, ENTITY);
            return json{
                { "entities", json_(ret) }
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

        void    p_dev_entities(WebHtml& h)
        {
            h.title() << "All Entities";
            dev_table(h, all_entities(Sorted::YES));
        }

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_entity_pages()
        {
            reg_webpage<p_api_entity>("/api/entity").argument("ID", "Entity ID");
            reg_webpage<p_api_entity_key>("/api/entity/key").argument("ID", "Entity ID");
            reg_webpage<p_api_entities>("/api/entities");

            reg_webtemplate("/entities", wksp::shared("www/entities.ht"sv));
            
            reg_webpage<p_dev_entities>("/dev/entities");
        }
}
