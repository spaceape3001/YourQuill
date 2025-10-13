////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/character/CharacterJson.hpp>
#include <mithril/character/CharacterSearch.hpp>
#include <mithril/wksp/Workspace.hpp>

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_character(WebContext& ctx)
        {
            Character    v   = arg::character(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_character_key(WebContext& ctx)
        {
            Character    v   = arg::character(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_characters(WebContext& ctx)
        {
            CharacterVector  ret = search(ctx, CHARACTER);
            return json{
                { "characters", json_(ret) }
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

        void    p_dev_characters(WebHtml& h)
        {
            h.title() << "All Characters";
            dev_table(h, all_characters(Sorted::YES));
        }

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_character_pages()
        {
            reg_webpage<p_api_character>("/api/character").argument("ID", "Character ID");
            reg_webpage<p_api_character_key>("/api/character/key").argument("ID", "Character ID");
            reg_webpage<p_api_characters>("/api/characters");

            //reg_webpage<p_characters>("/characters");

            reg_webpage<p_dev_characters>("/dev/characters");
            reg_webtemplate("/character", wksp::shared("www/character.ht"sv));
            reg_webtemplate("/characters", wksp::shared("www/characters.ht"sv));
        }
        
}
