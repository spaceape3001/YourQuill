////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uAtom.hpp"
#include "uClass.hpp"
#include <basic/Vector.hpp>
#include <kernel/atom/AtomCDB.hpp>
#include <kernel/db/SQ.hpp>

namespace yq {
    UAtom&  uget(Atom a)
    {
        static Vector<UAtom*>   s_data;
        s_data.resize_if_under(a.id+1, 65536, nullptr);
        UAtom*& p   = s_data[a.id];
        if(!p)
            p       = new UAtom(a);
        return *p;
    }
    
    using ClsHopsMap   = std::map<Class,uint64_t>;
    
    void    determine_hops(ClsHopsMap& hops, const ClassSet& cset, uint64_t depth=0)
    {
        for(Class c : cset){
            if(hops.contains(c))
                continue;
            hops[c]     = depth;
            determine_hops(hops, uget(c).base.defined, depth+1);
        }
    }
    
    void    update_aclasses(Atom at, Document doc, const ClsHopsMap& nmap)
    {
        ClsHopsMap    omap = make_map(cdb::classes_and_hops(at, doc));
        map_difference_exec(omap, nmap, 
            [&](const ClassU64Pair& pp) {
                static thread_local SQ  d("DELETE FROM AClasses WHERE atom=? AND doc=? AND class=?");
                auto d_af   = d.af();
                d.bind(1, at.id);
                d.bind(2, doc.id);
                d.bind(3, pp.first.id);
                d.exec();
            },
            
            [&](const ClassU64Pair& pp) {
                static thread_local SQ  u("UPDATE AClasses SET hops=? WHERE atom=? AND doc=? AND class=?");
                auto u_af   = u.af();
                u.bind(1, pp.second);
                u.bind(2, at.id);
                u.bind(3, doc.id);
                u.bind(4, pp.first.id);
                u.exec();
            },
            
        
            [&](const ClassU64Pair& pp){
                //  it's an addition
                static thread_local SQ  i("INSERT INTO AClasses (atom, doc, class, hops) VALUES (?,?,?,?)");
                auto i_af   = i.af();
                i.bind(1, at.id);
                i.bind(2, doc.id);
                i.bind(3, pp.first.id);
                i.bind(4, pp.second);
                i.exec();
            }
        );
    }
    

    void    update_attributes(Atom at, Document doc, const Attribute::Report& rep, cdb_options_t, const ClassSet& cset)
    {
//yInfo() << "Update on " << cdb::key(at) << " ... with " << cset.size() << " class(es)";
        ClassSet    oldcls    = make_set(cdb::classes(at));
        ClsHopsMap  newhops;
        determine_hops(newhops, cset);
        update_aclasses(at, doc, newhops);
        ClassSet    newcls    = make_set(cdb::classes(at));
        auto        clschgs   = add_remove(oldcls, newcls);
        
        
    }
}

