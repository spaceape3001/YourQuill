////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/atom/Atom.hpp>
#include <mithril/atom/AtomArg.hpp>
#include <mithril/atom/AtomCDB.hpp>
#include <mithril/class/Class.hpp>
#include <mithril/class/ClassArg.hpp>
#include <mithril/tag/Tag.hpp>
#include <mithril/tag/TagArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/web/WebSearcher.hpp>

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
