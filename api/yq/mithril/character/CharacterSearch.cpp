////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/character/Character.hpp>
#include <yq/mithril/character/CharacterCDB.hpp>
#include <yq/mithril/character/CharacterSearch.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    CharacterVector  search(WebContext&ctx, character_k)
    {
        WebSearcher<Character>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_characters(Sorted::YES);
        return ws.data;
    }
}
