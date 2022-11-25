////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AttributeDiff.hpp"
#include "AttributeCDB.hpp"

#include <basic/DiffEngine.hpp>
#include <basic/IntRange.hpp>

#include <kernel/bit/KeyValue.hpp>
#include <kernel/db/SQ.hpp>

#include <cassert>

namespace yq {

    namespace diff {
        namespace {
            bool    is_same(const agw::Attribute::KVUA& a, const KeyValue& b) 
            {
                return (a.key == b.key) && (a.value == b.data) && (a.uid == b.id);
            }
            
            bool    is_similar_kv(const agw::Attribute::KVUA& a, const KeyValue& b) 
            {
                return is_similar(a.key, b.key) && is_similar(a.value, b.data) && is_similar(a.uid, b.id);
            }
            
            void                add_all(std::vector<agw::Attribute::Diff>& items, const KVTree& kvs, ssize_t depth)
            {
                for(ssize_t i=0;i<(ssize_t) kvs.subs.size(); ++i){
                    auto& kv    = items.emplace_back();
                    kv.chg      = agw::Attribute::Diff::INSERT;
                    kv.key      = kvs.subs[i].key;
                    kv.value    = kvs.subs[i].data;
                    kv.uid      = kvs.subs[i].id;
                    kv.nidx     = i;
                    
                    if(depth && !kvs.subs[i].empty()){
                        kv.chg |= agw::Attribute::Diff::SUB;
                        add_all(kv.sub, kvs.subs[i], depth-1);
                    }
                }
            }

            void                remove_all(std::vector<agw::Attribute::Diff>& items, const std::vector<agw::Attribute::KVUA>& kvs, ssize_t depth)
            {
                for(const agw::Attribute::KVUA& k : kvs){
                    auto& kv    = items.emplace_back();
                    kv.chg      = agw::Attribute::Diff::DELETE;
                    kv.key      = k.key;
                    kv.value    = k.value;
                    kv.attr     = k.attr;
                    kv.uid      = k.uid;
                    kv.oidx     = k.idx;
                    
                    if(depth){
                        auto old    = cdb::kvua(k.attr);
                        if(!old.empty()){
                            remove_all(kv.sub, old, depth-1);
                            kv.chg |= agw::Attribute::Diff::SUB;
                        }
                    }
                }
            }
       
            bool                difference(std::vector<agw::Attribute::Diff>& items, const std::vector<agw::Attribute::KVUA>& old, const KVTree& kvs, ssize_t depth)
            {
                bool    chg = false;
                diff::diff_engine(old, kvs.subs, is_similar_kv, 
                    [&](SizeRange X, SizeRange Y){
                        assert(X.count() == Y.count());
                        for(ssize_t  r : SizeRange{0,std::min(X.count(),Y.count())}){
                            auto& c = items.emplace_back();
                            c.nidx  = Y.low+r;
                            c.oidx  = X.low+r;

                            auto& a = old[c.oidx];
                            auto& b = kvs.subs[c.nidx];
                            c.key   = b.key;
                            c.value = b.data;
                            c.uid   = b.id;
                            c.attr  = a.attr;
                            c.chg   = 0;
                        }
                    },
                    [&](SizeRange Y){
                        for(size_t y  : Y){
                            auto& kv    = items.emplace_back();
                            kv.chg      = agw::Attribute::Diff::INSERT;
                            kv.key      = kvs.subs[y].key;
                            kv.value    = kvs.subs[y].data;
                            kv.uid      = kvs.subs[y].id;
                            kv.nidx     = y;
                        }
                        chg     = true;
                    }, 
                    [&](SizeRange X){
                        for(size_t x  : X){
                            auto& kv    = items.emplace_back();
                            kv.chg      = agw::Attribute::Diff::DELETE;
                            kv.key      = old[x].key;
                            kv.value    = old[x].value;
                            kv.attr     = old[x].attr;
                            kv.uid      = old[x].uid;
                            kv.oidx     = x;
                        }
                        chg     = true;
                    } 
                );
                
                for(auto& r : items){
                    if(r.oidx != r.nidx)
                        r.chg         |= agw::Attribute::Diff::INDEX;
                    if(r.unchanged() && !is_same(old[r.oidx], kvs.subs[r.nidx])){
                        r.chg |= agw::Attribute::Diff::MODIFY;
                        chg  = true;
                    }
                }
                
                if(depth--){
                    for(auto& r : items){
                        if(difference(r.sub, cdb::kvua(r.attr), kvs.subs.valid(r.nidx) ?  kvs.subs[r.nidx] : KeyValue{}, depth)){
                            r.chg |= agw::Attribute::Diff::SUB;
                            chg = true;
                        }
                    }
                }
                return chg;
            }
        }

        agw::Attribute::Report  additions(Document doc, const KVTree& kv, ssize_t depth)
        {
            agw::Attribute::Report    ret;
            ret.changed     = !kv.empty();
            add_all(ret.items, kv, depth);
            ret.document    = doc;
            return ret;
        }

        agw::Attribute::Report changes(Document doc, const KVTree& kv, ssize_t depth)
        {
            agw::Attribute::Report    ret;
            ret.changed     = difference(ret.items, cdb::kvua(doc), kv, depth);
            ret.document    = doc;
            return ret;
        }
        
        agw::Attribute::Report changes(agw::Attribute attr, const KVTree& kv, ssize_t depth)
        {
            agw::Attribute::Report    ret;
            ret.changed     = difference(ret.items, cdb::kvua(attr), kv, depth);
            ret.parent      = attr;
            ret.document    = cdb::document(attr);
            return ret;
        }
        
        agw::Attribute::Report changes(const std::vector<agw::Attribute::KVUA>& old, const KVTree&kv, ssize_t depth)
        {
            agw::Attribute::Report    ret;
            ret.changed =  difference(ret.items, old, kv, depth);
            return ret;
        }

    }
    
    namespace cdb {
        void        u_insert(Document doc, agw::Attribute par, std::vector<agw::Attribute::Diff>& items)
        {
            for(auto& d : items){
                if((d.chg & agw::Attribute::Diff::XMASK) == agw::Attribute::Diff::INSERT)
                    d.attr  = db_attribute(doc, par, d.nidx, d.key, d.uid, d.value);
                u_insert(doc, d.attr, d.sub);
            }
        }
        
        void        u_index(const std::vector<agw::Attribute::Diff>& items)
        {
            static thread_local SQ u("UPDATE Attributes SET idx=? WHERE id=?");
            for(auto& d : items){
                if(d.attr && d.indexed() && !d.deleted())
                    u.exec(d.nidx, d.attr.id);
                if(d.subchanged())
                    u_index(d.sub);
            }
        }
        
        void        u_delete(const std::vector<agw::Attribute::Diff>&items)
        {
            static thread_local SQ x("DELETE FROM Attributes WHERE id=?");
            for(auto& d : items){
                if(d.deleted())
                    x.exec(d.attr.id);
                if(d.subchanged())
                    u_delete(d.sub);
            }
        }        
    }

    size_t  agw::Attribute::Diff::total_items() const
    {
        size_t  c   = sub.size();
        for(const Diff &d : sub)
            c += d.total_items();
        return c;
    }

    size_t  agw::Attribute::Report::total_items() const
    {
        size_t   c = items.size();
        for(const Diff& d : items)
            c += d.total_items();
        return c;
    }

    void    agw::Attribute::Report::exec_inserts()
    {
        cdb::u_insert(document, parent, items);
    }
    
    void    agw::Attribute::Report::exec_reindex()
    {
        cdb::u_index(items);
    }
    
    void    agw::Attribute::Report::exec_removals()
    {
        cdb::u_delete(items);
    }
}
