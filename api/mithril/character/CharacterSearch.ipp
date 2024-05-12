////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/character/Character.hpp>
#include <mithril/character/CharacterCDB.hpp>
#include <mithril/character/CharacterSearch.hpp>

namespace yq::mithril {
    CharacterVector  search(WebContext&ctx, character_t)
    {
        WebSearcher<Character>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_characters(Sorted::YES);
        return ws.data;
    }
}
