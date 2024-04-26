////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/AtomCDB.hpp>

namespace {
    void    u_atom(Atom a, std::span<Attribute::Diff> attrs, Change chg, Image icon={})
    {
        if(!a)
            return ;
            
        Atom::Diff      x { 
            .x = a, 
            .id = a.id, 
            .doc = cdb::document(a), 
            .chg=chg, 
            .key = cdb::key(a), 
            .diffs=attrs 
        };
        
        if(chg != Change::Added){
            x.classes.from      = cdb::classes_set(a);
            x.tags.from         = cdb::tags_set(a);
            x.title.from        = cdb::title(a);
            x.abbreviation.from = cdb::abbreviation(a);
        }
    }

    void    s4_atom()
    {
        //  assimilate edges
    }
}
