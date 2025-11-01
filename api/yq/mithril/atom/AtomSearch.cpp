////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/atom/Atom.hpp>
#include <yq/mithril/atom/AtomArg.hpp>
#include <yq/mithril/atom/AtomCDB.hpp>
#include <yq/mithril/class/Class.hpp>
#include <yq/mithril/class/ClassArg.hpp>
#include <yq/mithril/tag/Tag.hpp>
#include <yq/mithril/tag/TagArg.hpp>
#include <yq/mithril/web/WebContext.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    bool        search(WebSearcher<Atom>&ws, class_k)
    {
        return search(ws, "class", 
            [](std::string_view s) -> Class 
            { 
                return arg::class_(s);
            },
            [](Class c) -> auto 
            {
                return cdb::all_atoms(c, Sorted::YES);
            },
            [](Class c, Atom a) -> bool
            {
                return !cdb::is(a,c);
            }
        );
    }
    
    bool        search(WebSearcher<Atom>&ws, tag_k)
    {
        return search(ws, "tag",
            [](std::string_view s) -> Tag
            {
                return arg::tag(s);
            }, 
            [](Tag t) -> auto
            {
                return cdb::all_atoms(t, Sorted::YES);
            },
            [](Tag t, Atom a) -> bool 
            {
                return !cdb::is(a, t);
            }
        );
    }

    AtomVector  search(WebContext&ctx, atom_k)
    {
        WebSearcher<Atom>   ws(ctx);
        search(ws, TAG);
        search(ws, CLASS);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_atoms(Sorted::YES);
        return ws.data;
    }
}
